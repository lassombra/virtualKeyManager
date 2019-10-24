#include "pch.h"
#include "InputRandomDriver.h"

const InputActions* actions;

extern "C" DRIVER_API const InputDriver& __cdecl getInputDriver(const InputActions &ActionSpec) {
	actions = &ActionSpec;
	return InputDriver();
}
extern "C" DRIVER_API void __cdecl releaseInputDriver() {

}