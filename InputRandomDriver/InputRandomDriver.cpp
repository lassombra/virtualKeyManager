#include "pch.h"
#include "InputRandomDriver.h"
#include <mutex>
#include <chrono>
#include <thread>

using namespace std;

const InputActions* actions;
const Capabilities* caps;
InputDriver driver;
mutex mtx;
BOOL running = FALSE;
BOOL stopping = FALSE;
condition_variable waitStopping;
condition_variable waitStop;
vector<BYTE> keysDown;

void sendRandomKey() {
	unique_lock<mutex> lck(mtx);
	BYTE keyToPress = rand() % caps->Buttons;
	BYTE keyToRelease = 0;
	if (keysDown.size() > 0) {
		int releaseIndex = rand() % keysDown.size();
		keyToRelease = keysDown[releaseIndex];
		keysDown.erase(keysDown.begin() + releaseIndex);
	}
	keysDown.push_back(keyToPress);
	lck.unlock();
	actions->setDown(keyToPress);
	if (keyToRelease > 0)
		actions->setUp(keyToRelease);
}
void inputLoop() {
	unique_lock<mutex> lck(mtx);
	running = TRUE;
	while (!stopping) {
		if (caps != nullptr) {
			lck.unlock();
			sendRandomKey();
			lck.lock();
		}
		waitStop.wait_for(lck, chrono::seconds(10));
	}
	running = FALSE;
	waitStopping.notify_all();
}
void __cdecl setCapabilities(const Capabilities& capsSet) {
	unique_lock<mutex> lck(mtx);
	caps = &capsSet;
}
BOOL __cdecl activate(const InputActions& actionSpec) {
	unique_lock<mutex> lck(mtx);
	actions = &actionSpec;
	while (running) waitStopping.wait(lck);
	stopping = FALSE;
	thread(inputLoop).detach();
	return TRUE;
}
BOOL __cdecl deactivate() {
	unique_lock<mutex> lck(mtx);
	stopping = TRUE;
	waitStop.notify_all();
	waitStopping.wait(lck);
	keysDown.clear();
	return TRUE;
}

extern "C" DRIVER_API const InputDriver& __cdecl getInputDriver() {
	driver.setCapabilities = &setCapabilities;
	driver.activate = &activate;
	driver.deactivate = &deactivate;
	driver.name = L"Random Input Driver";
	return driver;
}
extern "C" DRIVER_API void __cdecl releaseInputDriver() {

}