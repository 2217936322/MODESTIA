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
    int* PredictionRandomSeed;
    std::add_pointer_t<const char** __fastcall(const char* playerModelName)> GetPlayerViewmodelArmConfigForPlayerModel;
};

extern Memory g_Memory;