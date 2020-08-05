#include "Memory.hpp"

Memory g_Memory;

template <typename T>
static constexpr auto relativeToAbsolute(uintptr_t address) noexcept
{
    return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

Memory::Memory()
{

    auto temp = reinterpret_cast<std::uintptr_t*>(findPattern(L"client", "\xB9????\xE8????\x8B\x5D\x08") + 1);
    hud = *temp;
    findHudElement = relativeToAbsolute<decltype(findHudElement)>(reinterpret_cast<uintptr_t>(temp) + 5);
    clearHudWeapon = reinterpret_cast<decltype(clearHudWeapon)>(findPattern(L"client", "\x55\x8B\xEC\x51\x53\x56\x8B\x75\x08\x8B\xD9\x57\x6B\xFE\x2C"));
    equipWearable = reinterpret_cast<decltype(equipWearable)>(findPattern(L"client", "\x55\x8B\xEC\x83\xEC\x10\x53\x8B\x5D\x08\x57\x8B\xF9"));
    getPlayerViewmodelArmConfigForPlayerModel = relativeToAbsolute<decltype(getPlayerViewmodelArmConfigForPlayerModel)>(findPattern(L"client", "\xE8????\x89\x87????\x6A\x00") + 1);
}