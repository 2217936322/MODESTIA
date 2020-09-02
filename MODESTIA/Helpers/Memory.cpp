#include "Memory.hpp"

#include "Utils.hpp"

Memory g_Memory;

template <typename T>
static constexpr auto RelativeToAbsolute(uint8_t* address)
{
    return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

Memory::Memory()
{
    auto temp = reinterpret_cast<std::uintptr_t*>(Utils::PatternScan(GetModuleHandle("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
    Hud = *temp;
    FindHudElement = RelativeToAbsolute<decltype(FindHudElement)>(reinterpret_cast<uint8_t*>(temp) + 5);
    ClearHudWeapon = reinterpret_cast<decltype(ClearHudWeapon)>(Utils::PatternScan(GetModuleHandle("client.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"));
    EquipWearable = reinterpret_cast<decltype(EquipWearable)>(Utils::PatternScan(GetModuleHandle("client.dll"), "55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9"));
    PredictionRandomSeed = *reinterpret_cast<int**>(Utils::PatternScan(GetModuleHandle("client.dll"), " 8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
    GetPlayerViewmodelArmConfigForPlayerModel = RelativeToAbsolute<decltype(GetPlayerViewmodelArmConfigForPlayerModel)>(Utils::PatternScan(GetModuleHandle("client.dll"), "E8 ? ? ? ? 89 87 ? ? ? ? 6A ?") + 1);
}