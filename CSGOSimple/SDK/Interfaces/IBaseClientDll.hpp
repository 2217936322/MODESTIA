#pragma once

#include "../Misc/GlobalVars.hpp"
#include "../Misc/ClientClass.hpp"

class IBaseClientDLL
{
public:
	virtual int              Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* globals) = 0;
	virtual int              Disconnect(void) = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* globals) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown(void) = 0;
	virtual void             LevelInitPreEntity(char const* mapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown(void) = 0;
	virtual ClientClass* GetAllClasses(void) = 0;

	ClientClass* GetClientClasses()
	{
		using Fn = ClientClass* (__thiscall*)(IBaseClientDLL*);
		return (*(Fn**)this)[8](this);
	}

	bool DispatchUserMessage(int messageType, int arg, int arg1, void* data)
	{
		using Fn = bool* (__thiscall*)(void*, int, int, int, void*);
		return CallVFunction<Fn>(this, 38)(this, messageType, arg, arg1, data);
	}
};