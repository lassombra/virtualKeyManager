#include "pch.h"
#include "InputRandomDriver.h"
#include <mutex>

using namespace std;

const InputActions* actions;
atomic<const Capabilities*> caps = nullptr;
InputDriver driver;
std::shared_ptr<Interruptible::thread> t;

void __cdecl setCapabilities(const Capabilities& capsSet) {
	caps = &capsSet;
}
BOOL __cdecl activate(const InputActions& actionSpec) {
	actions = &actionSpec;
	t.reset();
	t.reset(new Interruptible::thread(inputLoop));
	return TRUE;
}
BOOL __cdecl deactivate() {
	t.reset();
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
	t.reset();
}