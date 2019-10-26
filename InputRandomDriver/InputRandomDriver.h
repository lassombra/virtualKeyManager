#pragma once

#include "InputDriverPlugin.h"
#include "../Util/Interruptible.h"
#include <atomic>

extern const InputActions* actions;
extern std::atomic<const Capabilities*> caps;

void sendRandomKeys();
void inputLoop();
BOOL __cdecl deactivate();
BOOL __cdecl activate(const InputActions& actionSpec);
void __cdecl setCapabilities(const Capabilities& capsSet);
