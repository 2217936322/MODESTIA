#include "Platform.hpp"

#include <vector>
#include <algorithm>
#include <Windows.h>
#include <psapi.h>

#include "../../SDK/SDK.hpp"

auto Platform::GetExport(const char* moduleName, const char* exportName) -> void*
{
	HMODULE mod;
	while (!((mod = GetModuleHandleA(moduleName))))
		Sleep(100);

	return reinterpret_cast<void*>(GetProcAddress(mod, exportName));
}

auto Platform::GetInterface(const char* moduleName, const char* interfaceName) -> void*
{
	const auto addr = GetExport(moduleName, "CreateInterface");
	const auto createInterfaceFn = reinterpret_cast<CreateInterfaceFn>(addr);

	return createInterfaceFn(interfaceName, nullptr);
}

auto Platform::GetModuleInfo(const char* moduleName) -> std::pair<std::uintptr_t, std::size_t>
{
	const auto module = GetModuleHandleA(moduleName);
	if (!module)
		return { 0, 0 };

	MODULEINFO moduleInfo;
	K32GetModuleInformation(GetCurrentProcess(), module, &moduleInfo, sizeof(MODULEINFO));
	return { std::uintptr_t(moduleInfo.lpBaseOfDll), moduleInfo.SizeOfImage };
}

auto Platform::IsCodePtr(void* ptr) -> bool
{
	constexpr const DWORD protectFlags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION out;
	VirtualQuery(ptr, &out, sizeof out);

	return out.Type
		&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		&& out.Protect & protectFlags;
}