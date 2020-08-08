#include "Memory.hpp"

Memory g_Memory;

template <typename T>
static constexpr auto RelativeToAbsolute(uintptr_t address) noexcept
{
    return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

Memory::Memory()
{
    auto temp = reinterpret_cast<std::uintptr_t*>(FindPattern(L"client", "\xB9????\xE8????\x8B\x5D\x08") + 1);

    Hud = *temp;

    FindHudElement = RelativeToAbsolute<decltype(FindHudElement)>(reinterpret_cast<uintptr_t>(temp) + 5);

    ClearHudWeapon = reinterpret_cast<decltype(ClearHudWeapon)>(FindPattern(L"client", "\x55\x8B\xEC\x51\x53\x56\x8B\x75\x08\x8B\xD9\x57\x6B\xFE\x2C"));

    EquipWearable = reinterpret_cast<decltype(EquipWearable)>(FindPattern(L"client", "\x55\x8B\xEC\x83\xEC\x10\x53\x8B\x5D\x08\x57\x8B\xF9"));

    GetPlayerViewmodelArmConfigForPlayerModel = RelativeToAbsolute<decltype(GetPlayerViewmodelArmConfigForPlayerModel)>(FindPattern(L"client", "\xE8????\x89\x87????\x6A\x00") + 1);
}