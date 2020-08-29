#include <Windows.h>
#include <chrono>
#include <thread>

#include "Hooks.hpp"

#include "SDK/SDK.hpp"
#include "Menu/Menu.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/InputSystem.hpp"
#include "Helpers/Configs/ConfigSys.hpp"

DWORD WINAPI Initialize(LPVOID base) 
{
	if (Utils::WaitForModules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll", L"serverbrowser.dll" }) == WAIT_TIMEOUT)
	{
		return FALSE;
	}

#ifdef _DEBUG
	Utils::AttachConsole();
#endif

	try 
	{
		Config->Setup();
		Utils::ConsolePrint("Initializing...\n");

		Interfaces::Initialize();
		Interfaces::Dump();
		NetvarSys::Get().Initialize();
		InputSystem::Get().Initialize();
		Hooks::Initialize();

		InputSystem::Get().RegisterHotkey(g_Configs.misc.unloadKey, [base](){ Unload = true; });

		Utils::ConsolePrint("Finished.\n");

		while (!Unload)
			Sleep(1000);

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);

	}
	catch (const std::exception& ex) 
	{
		Utils::ConsolePrint("An error occured during initialization:\n");
		Utils::ConsolePrint("%s\n", ex.what());
		Utils::ConsolePrint("Press any key to exit.\n");
		Utils::ConsoleReadKey();
		Utils::DetachConsole();

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
	}
}

BOOL WINAPI Release()
{
#ifdef _DEBUG
	Utils::DetachConsole();
#endif

	Hooks::Release();

	return TRUE;
}

BOOL WINAPI DllMain(_In_ HINSTANCE instance, _In_ DWORD reason, _In_opt_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(instance);
		CreateThread(nullptr, 0, Initialize, instance, 0, nullptr);
		return TRUE;

	case DLL_PROCESS_DETACH:
		if (reserved == nullptr)
			return Release();
		return TRUE;

	default:
		return TRUE;
	}
}
