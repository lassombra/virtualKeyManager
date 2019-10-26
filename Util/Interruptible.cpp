#include "pch.h"
#include "Interruptible.h"

namespace Interruptible {
	namespace this_thread {
		thread_local std::shared_ptr<std::atomic<bool>> interrupted;
		thread_local std::shared_ptr<std::condition_variable> interrupt_monit;
		thread_local std::mutex interrupt_mtx;
	};
}