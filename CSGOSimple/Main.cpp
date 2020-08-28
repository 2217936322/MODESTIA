#include <Windows.h>
#include <chrono>
#include <thread>

#include "Hooks.hpp"

#include "SDK/SDK.hpp"
#include "Menu/Menu.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/InputSystem.hpp"
#include "Helpers/Configs/ConfigSys.hpp"

DWORD WINAPI Initialize(void* instance) 
{
	while (!GetModuleHandle("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	Utils::AttachConsole();
#endif

	try 
	{
		Config->Setup();
		Interfaces::Initialize();
		Interfaces::Dump();
		NetvarSys::Get().Initialize();
		InputSystem::Get().Initialize();
		Hooks::Initialize();
	}

	catch (const std::runtime_error & error) 
	{
		MessageBoxA(NULL, error.what(), "MODE$TIA Error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

}

BOOL WINAPI Release()
{
	Hooks::Release();

#ifdef _DEBUG
	Utils::DetachConsole();
#endif

	return TRUE;
}

BOOL APIENTRY DllMain(void* instance, uintptr_t reason, void* reserved) 
{
	DisableThreadLibraryCalls(static_cast<HMODULE>(instance));

	switch (reason) 
	{
	case DLL_PROCESS_ATTACH:
		if (auto handle = CreateThread(NULL, NULL, Initialize, instance, NULL, NULL))
			CloseHandle(handle);
		break;

	case DLL_PROCESS_DETACH:
		Release();
		break;
	}

	return true;
}