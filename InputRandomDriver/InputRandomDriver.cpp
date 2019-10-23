#include "InputRandomDriver.h"

extern "C" DRIVER_API InputDriver * __cdecl getInputDriver(void) {
	return &InputDriver();
}

extern "C" DRIVER_API void __cdecl releaseInputDriver(InputDriver * driver) {}