// OutputVJoyDriver.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "OutputVJoyDriver.h"


// This is an example of an exported function.
OUTPUTDRIVER_API const wchar_t* fnOutputVJoyDriver(void)
{
    return L"Initialized";
}

