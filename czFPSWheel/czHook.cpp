#include "czHook.h"

czHook::czHook() //empty default constructor
{
	address = 0;
	destination = 0;
	trampoline = 0;
	hook = 0;
}
DWORD* czHook::init(DWORD addr, DWORD  dest, uint64_t* trampoline)
{
	return czApi::addHook((char*)addr, (char*)dest, trampoline);
}
//
//template<typename X, typename T>
//czHook::czHook(X addr, T dest) {
//	hook = czApi::addHook((char*)addr, (char*)dest, &trampoline);
//	destination = dest;
//	address = addr;
//}

void czHook::removeHook()
{
	if (hook != nullptr)
		czApi::removeHook(hook);
	else
		czApi::writeConsole("Failed to remove hook -- nullptr");
}