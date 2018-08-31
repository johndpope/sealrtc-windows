#pragma once

#include <imagehlp.h>
#pragma comment(lib, "dbghelp.lib")

LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);