#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <type_traits>
class InterruptibleThread;

namespace Interruptible {

	struct interruptedException : std::exception {};

	namespace this_thread {
		extern thread_local std::shared_ptr<std::atomic<bool>> interrupted;
		extern thread_local std::shared_ptr<std::condition_variable> interrupt_monit;
		extern thread_local std::mutex interrupt_mtx;
		template <typename T, typename U>
		void sleep_for(std::chrono::duration<T, U> duration) {
			auto lock = std::unique_lock<std::mutex>(interrupt_mtx);
			auto now = std::chrono::steady_clock::now();
			auto until = now + duration;
			while (!*interrupted && (now < until)) {
				interrupt_monit->wait_until(lock, until);
				now = std::chrono::steady_clock::now();
			}
			if (*interrupted) {
				throw interruptedException();
			}
		}
	};

	template <class _Fn, class... _Args>
	void threadInit(const std::shared_ptr<std::atomic<bool>> interrupted, const std::shared_ptr<std::condition_variable> interrupt_monit, _Fn&& _Fx, _Args&&... args) {
		this_thread::interrupted = interrupted;
		this_thread::interrupt_monit = interrupt_monit;
		try {
			std::invoke(_Fx, std::forward<_Args>(args)...);
		}
		catch (interruptedException e) {}
	}

	class thread
	{
		std::shared_ptr<std::atomic<bool>> _interrupted;
		std::shared_ptr<std::condition_variable> _interrupt_monit;
		std::thread _t;
	public:
		thread() {
		}
		~thread() {
			this->interrupt();
			if (this->_t.joinable()) {
				this->_t.join();
			}
		}
		void interrupt() {
			*(this->_interrupted) = true;
			this->_interrupt_monit->notify_all();
		}

		template <class _Fn, class... _Args>
		explicit thread(_Fn&& _Fx, _Args&&... args) {
			this->_interrupted = std::make_shared<std::atomic<bool>>(false);
			this->_interrupt_monit = std::make_shared<std::condition_variable>();
			this->_t = std::move(std::thread(threadInit<_Fn, _Args...>, this->_interrupted, this->_interrupt_monit, std::ref(_Fx), std::forward<_Args>(args)...));
		}

		thread(thread&& _mv_t) noexcept {
			this->_t = std::move(_mv_t._t);
			this->_interrupted = std::move(_mv_t._interrupted);
			this->_interrupt_monit = std::move(_mv_t._interrupt_monit);
		}
	};

}