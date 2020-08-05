#pragma once

#include <basetsd.h>

class IClientLeafSystem
{
public:
	void CreateRenderableHandle(void* obj)
	{
		typedef void(__thiscall* Fn)(void*, int, int, char, signed int, char);
		CallVFunction<Fn>(this, 0)(this, reinterpret_cast<uintptr_t>(obj) + 0x4, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF);
	}
};