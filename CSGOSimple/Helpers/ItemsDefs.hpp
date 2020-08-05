#pragma once

#include <map>
#include <vector>

#include "../SDK/CSGOStructs.hpp"

static auto IsKnife(const int i) -> bool
{
	return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
}

struct C_WeaponInfo
{
	constexpr C_WeaponInfo(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}

	const char* model;
	const char* icon;
};

struct C_WeaponName
{
	constexpr C_WeaponName(const int definitionIndex, const char* name) :
		definitionIndex(definitionIndex),
		name(name)
	{}

	int definitionIndex = 0;
	const char* name = nullptr;
};

extern const std::map<size_t, C_WeaponInfo> k_WeaponInfo;
extern const std::vector<C_WeaponName> k_KnifeNames;
extern const std::vector<C_WeaponName> k_GloveNames;
extern const std::vector<C_WeaponName> k_WeaponNames;