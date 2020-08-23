#pragma once

#include <Windows.h>
#include <string>
#include <initializer_list>

#include "../SDK/SDK.hpp"

namespace Utils 
{
    unsigned int FindInDataMap(datamap_t* map, const char* name);
    void AttachConsole();
    void DetachConsole();
    bool ConsolePrint(const char* fmt, ...);
    char ConsoleReadKey();
    int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);
    std::uint8_t* PatternScan(void* module, const char* signature);
}
