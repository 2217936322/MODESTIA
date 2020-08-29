#pragma once

#include <array>

#include "SDK.hpp"

#include "../Helpers/Utils.hpp"

#define NETVAR(type, name, table, netvar) type& name##() const				\
{																			\
	static int _##name = NetvarSys::Get().GetOffset(table, netvar);			\
	return *(type*)((uintptr_t)this + _##name);								\
}

#define PNETVAR(type, name, table, netvar) type* name##() const				\
{																			\
	static int _##name = NetvarSys::Get().GetOffset(table, netvar);			\
	return (type*)((uintptr_t)this + _##name);								\
}

#define NETPROP(name, table, netvar) static RecvProp* name()				\
{																			\
	static auto prop_ptr = NetvarSys::Get().GetNetvarProp(table,netvar);	\
	return prop_ptr;														\
}

class CEconItemView
{
private:
	using str_32 = char[32];
public:
	NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(int32_t, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel");
	NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(str_32, m_szCustomName, "DT_BaseAttributableItem", "m_szCustomName");
	NETVAR(short, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
};

class CBaseEntity : public IClientEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(int32_t, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(int32_t, m_nSmokeEffectTickBegin, "DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin");
	NETVAR(int32_t, m_nExplodeEffectTickBegin, "DT_BaseCSGrenadeProjectile", "m_nExplodeEffectTickBegin");
	NETVAR(int32_t, m_nBombSite, "DT_PlantedC4", "m_nBombSite");
	NETVAR(float_t, m_flTimerLength, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float_t, m_flDefuseLength, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float_t, m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
	NETVAR(float_t, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(Vector, m_vecMins, "DT_BaseEntity", "m_vecMins");
	NETVAR(Vector, m_vecMaxs, "DT_BaseEntity", "m_vecMaxs");
	NETVAR(Vector, m_vecAngles, "DT_BaseEntity", "m_vecAngles");
	NETVAR(CHandle<CBasePlayer>, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(CHandle<CBasePlayer>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
	NETVAR(bool, m_bShouldGlow, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(bool, m_bSpotted, "DT_BaseEntity", "m_bSpotted");
	NETVAR(bool, m_bBombTicking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");

	void SetModelIndex(int modelIndex);
};

class CBaseWeaponWorldModel : public CBaseEntity
{
public:
};

class CBaseAttributableItem : public CBaseEntity
{
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_nModelIndex, "DT_BaseWeaponWorldModel", "m_nModelIndex");
	NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(CHandle<CBaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(CEconItemView, m_Item2, "DT_BaseAttributableItem", "m_Item");
	NETVAR(char, m_szCustomName, "DT_BaseAttributableItem", "m_szCustomName");

	CEconItemView& m_Item()
	{
		return *(CEconItemView*)this;
	}
};

class CBaseCombatWeapon : public CBaseAttributableItem
{
public:
	NETVAR(int16_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2");
	NETVAR(int32_t, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_iWorldDroppedModelIndex, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(int32_t, m_weaponMode, "DT_WeaponCSBase", "m_weaponMode");
	NETVAR(int32_t, m_zoomLevel, "DT_WeaponCSBaseGun", "m_zoomLevel");
	NETVAR(float_t, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(float_t, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(float_t, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flPostponeFireReadyTime, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<CBaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
};

class CBasePlayer : public CBaseEntity
{
public:
	NETVAR(int32_t, m_iHealth, "DT_BasePlayer", "m_iHealth");
	NETVAR(int32_t, m_lifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(int32_t, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int32_t, m_nTickBase, "DT_BasePlayer", "m_nTickBase");
	NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(int32_t, m_nSequence, "DT_BaseViewModel", "m_nSequence");
	NETVAR(int32_t, m_iFOV, "DT_BasePlayer", "m_iFOV");
	NETVAR(int32_t, m_iDefaultFOV, "DT_BasePlayer", "m_iDefaultFOV");
	NETVAR(int32_t, m_nBody, "DT_BaseAnimating", "m_nBody")
	NETVAR(int32_t, m_hRagdoll, "DT_CSPlayer", "m_hRagdoll");
	NETVAR(int32_t, m_nSurvivalTeam, "DT_CSPlayer", "m_nSurvivalTeam");
	NETVAR(int32_t, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(int32_t, m_iAccount, "DT_CSPlayer", "m_iAccount");
	NETVAR(int32_t, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(int32_t, m_iObserverMode, "DT_BasePlayer", "m_iObserverMode");
	NETVAR(float_t, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(float_t, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float_t, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(float_t, m_flCycle, "DT_ServerAnimationData", "m_flCycle");
	NETVAR(float_t, m_flDuckSpeed, "DT_BaseEntity", "m_flDuckSpeed");
	NETVAR(float_t, m_flDuckAmount, "DT_BasePlayer", "m_flDuckAmount");
	NETVAR(float_t, m_flMaxspeed, "DT_BasePlayer", "m_flMaxspeed");
	NETVAR(float_t, m_flFlashMaxAlpha, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(QAngle, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(QAngle, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(QAngle, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(QAngle, m_angAbsAngles, "DT_BaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "DT_BaseEntity", "m_angAbsOrigin");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(CHandle<CBasePlayer>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(CHandle<CBaseViewModel>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(CHandle<CBaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(CHandle<CBaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	PNETVAR(CHandle<CBaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(int32_t, m_hMyWearables, "DT_BaseCombatCharacter", "m_hMyWearables");
	PNETVAR(char, m_szLastPlaceName, "DT_BasePlayer", "m_szLastPlaceName");
	NETVAR(bool, m_bHasDefuser, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(bool, m_bIsDefusing, "DT_CSPlayer", "m_bIsDefusing");
	NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(bool, m_bHasHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "DT_CSPlayer", "m_bIsScoped");

	bool IsAlive();
	int Handle();
	int m_nMoveType();
};

class CBaseViewModel : public CBaseEntity
{
public:
	NETVAR(int32_t, m_nIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(int32_t, m_hViewModel, "DT_BasePlayer", "m_hViewModel");
	NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, m_hOwner2, "DT_BaseViewModel", "m_hOwner")
	NETVAR(CHandle<CBaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<CBaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	NETVAR(CHandle<CBasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
	NETPROP(m_nSequence, "DT_BaseViewModel", "m_nSequence");
};

class CCSPlayerResource
{
public:
	PNETVAR(int32_t, m_nActiveCoinRank, "CCSPlayerResource", "m_nActiveCoinRank");
	PNETVAR(int32_t, m_nMusicID, "CCSPlayerResource", "m_nMusicID");
	PNETVAR(int32_t, m_nPersonaDataPublicLevel, "CCSPlayerResource", "m_nPersonaDataPublicLevel");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsLeader, "CCSPlayerResource", "m_nPersonaDataPublicCommendsLeader");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsTeacher, "CCSPlayerResource", "m_nPersonaDataPublicCommendsTeacher");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsFriendly, "CCSPlayerResource", "m_nPersonaDataPublicCommendsFriendly");
	PNETVAR(int32_t, m_iCompetitiveRanking, "CCSPlayerResource", "m_iCompetitiveRanking");
	PNETVAR(int32_t, m_iCompetitiveWins, "CCSPlayerResource", "m_iCompetitiveWins");
	PNETVAR(int32_t, m_iPlayerVIP, "CCSPlayerResource", "m_iPlayerVIP");
	PNETVAR(int32_t, m_iMVPs, "CCSPlayerResource", "m_iMVPs");
	PNETVAR(int32_t, m_iScore, "CCSPlayerResource", "m_iScore");
};

class CGameRules
{
public:
	NETVAR(int, m_iMatchStats_PlayersAlive_T, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_T");
	NETVAR(int, m_iMatchStats_PlayersAlive_CT, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_CT");
	NETVAR(int, m_iRoundTime, "DT_CSGameRulesProxy", "m_iRoundTime");
	NETVAR(bool, m_bFreezePeriod, "DT_CSGameRulesProxy", "m_bBombDropped");
	NETVAR(bool, m_bIsValveDS, "DT_CSGameRulesProxy", "m_bIsValveDS");
	NETVAR(bool, m_bBombDropped, "DT_CSGameRulesProxy", "m_bBombDropped");
	NETVAR(bool, m_bBombPlanted, "DT_CSGameRulesProxy", "m_bBombPlanted");
	NETVAR(float, m_flSurvivalStartTime, "DT_CSGameRulesProxy", "m_flSurvivalStartTime");
};

extern CGameRules* g_GameRules;
