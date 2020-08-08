#pragma once

#include <map>
#include <vector>

#include "../SDK/CSGOStructs.hpp"

static bool IsKnife(const int i)
{
	return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
}

struct CWeaponInfo
{
	constexpr CWeaponInfo(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}

	const char* model;
	const char* icon;
};

struct CWeaponName
{
	constexpr CWeaponName(const int definitionIndex, const char* name) :
		definitionIndex(definitionIndex),
		name(name)
	{}

	int definitionIndex = 0;
	const char* name = nullptr;
};

extern const std::map<size_t, CWeaponInfo> k_WeaponInfo;
extern const std::vector<CWeaponName> k_KnifeNames;
extern const std::vector<CWeaponName> k_GloveNames;
extern const std::vector<CWeaponName> k_WeaponNames;