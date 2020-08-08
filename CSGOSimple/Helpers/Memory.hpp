#pragma once

#include <array>
#include <memory>
#include <string>
#include <type_traits>
#include <Windows.h>
#include <Psapi.h>

class Memory 
{
public:
    Memory();

    uintptr_t Hud;

    int* (__thiscall* FindHudElement)(uintptr_t, const char*);

    int(__thiscall* ClearHudWeapon)(int*, int);

    int(__thiscall* EquipWearable)(void* wearable, void* player);

    std::add_pointer_t<const char** __fastcall(const char* playerModelName)> GetPlayerViewmodelArmConfigForPlayerModel;

private:
    static std::uintptr_t FindPattern(const wchar_t* module, const char* pattern)
    {
        static auto id = 0;
        ++id;

        if (HMODULE moduleHandle = GetModuleHandleW(module)) 
        {
            if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), moduleHandle, &moduleInfo, sizeof(moduleInfo))) 
            {
                auto start = static_cast<const char*>(moduleInfo.lpBaseOfDll);
                const auto end = start + moduleInfo.SizeOfImage;

                auto first = start;
                auto second = pattern;

                while (first < end && *second) 
                {
                    if (*first == *second || *second == '?') 
                    {
                        ++first;
                        ++second;
                    }
                    else 
                    {
                        first = ++start;
                        second = pattern;
                    }
                }

                if (!*second)
                    return reinterpret_cast<std::uintptr_t>(start);
            }
        }

        MessageBoxA(NULL, ("Failed to find pattern #" + std::to_string(id) + '!').c_str(), "MODE$TIA", MB_OK | MB_ICONWARNING);
        return 0;
    }
};

extern Memory g_Memory;