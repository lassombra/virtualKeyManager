#include "pch.h"
#include "InputRandomDriver.h"
#include <chrono>
#include <mutex>
#include <set>


std::mutex mtx;
std::set<BYTE> keysDown;
std::atomic<bool> interrupted = false;
void sendRandomKeys() {
	std::set<BYTE> pressed;
	std::set<BYTE> released;
	// create a lock and lock it using a known shared mutex
	// this lock needs to be locked whenever manipulating
	// the shared keysDown array to avoid race conditions
	// as the set is not normally thread safe, nor is it's
	// iterators.
	std::unique_lock<std::mutex> lck(mtx);
	if (keysDown.size() > 0) {
		BYTE keysToRelease = rand() % (BYTE)keysDown.size();
		for (BYTE ii = 0; ii < keysToRelease; ii++) {
			BYTE releaseIndex = rand() % (BYTE)keysDown.size();
			auto it = keysDown.begin();
			for (BYTE idx = 0; idx < releaseIndex; idx++) {
				it++;
			}
			released.insert(*it);
			keysDown.erase(it);
		}
	}
	// we are done messing with the set for a moment, we'll leave it be
	lck.unlock();
	// since caps is atomic and not expected to change during the life of
	// one thread, this doesn't need a lock
	BYTE keysToPress = rand() % caps.load()->Buttons;
	for (BYTE i = 0; i < keysToPress; i++) {
		pressed.insert(rand() % caps.load()->Buttons);
	}
	for (auto it = released.begin(); it != released.end(); it++) {
		// release takes precedence, not much point "pressing" a button
		// that was already down
		if (pressed.count(*it) > 0) {
			pressed.erase(*it);
		}
	}
	// now we need a lock again because we are updating the keysDown structure
	lck.lock();
	for (auto it = pressed.begin(); it != pressed.end(); it++) {
		keysDown.insert(*it);
	}
	for (auto it = released.begin(); it != released.end(); it++) {
		keysDown.erase(*it);
	}
	lck.unlock();
	for (auto it = pressed.begin(); it != pressed.end(); it++) {
		if (!interrupted)
			actions->setDown(*it);
		// simulate delays in incoming button presses / releases
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 150));
	}
	for (auto it = released.begin(); it != released.end(); it++) {
		if (!interrupted)
			actions->setUp(*it);
		// simulate delays in incoming button presses / releases
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 150));
	}
}
void inputLoop() {
	// these two initializers are only here
	// in case this gets shut down and then started again.
	keysDown.clear();
	interrupted = false;
	while (true) {
		if (caps != nullptr) {
			// trigger sendRandomKeys in new thread
			// simulates non-blocking input type actions
			// due to the way sleeps are handled in this case, it's possible for
			// a new thread to be called while this one is running,
			// so they must be appropriately locking
			std::thread(sendRandomKeys).detach();
		}
		// sleep interruptibly for up to 10 seconds
		// note that this will not interrupt / join spun off threads, they can still send spurious
		// inputs.

		// As a result, because we **cannot** send inputs after this returns, we have an atomic bool
		// to let our triggers know to stop triggering.
		try {
			Interruptible::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10000));
		}
		catch (Interruptible::interruptedException e) {
			interrupted = true;
			throw e;
		}
	}
}