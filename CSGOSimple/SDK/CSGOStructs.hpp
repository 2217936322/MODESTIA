#pragma once

#include <array>

#include "SDK.hpp"
#include "../Helpers/Utils.hpp"

#define NETVAR(type, name, table, netvar)										\
    type& name##() const {														\
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);			\
        return *(type*)((uintptr_t)this + _##name);								\
    }

#define PNETVAR(type, name, table, netvar)										\
    type* name##() const {														\
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);			\
        return (type*)((uintptr_t)this + _##name);								\
    }


#define NETVAR_EX( type, name, table, netvar, extra )							\
  type& name##() const {														\
    static int _##name = NetvarSys::Get().GetOffset( table, netvar ) + extra;	\
    return *(type*)( (uintptr_t)this + _##name );								\
  }

#define NETPROP(name, table, netvar) static RecvProp* name()					\
{																				\
	static auto prop_ptr = NetvarSys::Get().GetNetvarProp(table,netvar);		\
	return prop_ptr;															\
}

struct datamap_t;
class AnimationLayer;
class CBasePlayerAnimState;
class CCSGOPlayerAnimState;
class C_BaseEntity;

class C_BaseEntity;

class CHudTexture
{
public:
	char szShortName[64];    
	char szTextureFile[64];  
	bool bRenderUsingFont;   
	bool bPrecached;         
	int8_t cCharacterInFont; 
	uint8_t pad_0083[1];     
	uint32_t hFont;          
	int32_t iTextureId;      
	float afTexCoords[4];    
	uint8_t pad_009C[16];    
};

class C_EconItemView
{
private:
	using str_32 = char[32];
public:
	NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(short, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
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

	std::string weapIconDef()
	{
		if (!this)
			return ("");
		int id = this->m_iItemDefinitionIndex();
		switch (id) {
		case WEAPON_KNIFE_T:
			return ("[");
		case WEAPON_DEAGLE:
			return ("A");
		case WEAPON_AUG:
			return ("U");
		case WEAPON_G3SG1:
			return ("X");
		case WEAPON_MAC10:
			return ("K");
		case WEAPON_P90:
			return ("P");
		case WEAPON_SSG08:
			return ("a");
		case WEAPON_SCAR20:
			return ("Y");
		case WEAPON_UMP45:
			return ("L");
		case WEAPON_ELITE:
			return ("B");
		case WEAPON_FAMAS:
			return ("R");
		case WEAPON_FIVESEVEN:
			return ("C");
		case WEAPON_GALILAR:
			return ("Q");
		case WEAPON_M4A1_SILENCER:
			return ("T");
		case WEAPON_M4A1:
			return ("S");
		case WEAPON_P250:
			return ("F");
		case WEAPON_M249:
			return ("g");
		case WEAPON_XM1014:
			return ("b");
		case WEAPON_GLOCK:
			return ("D");
		case WEAPON_USP_SILENCER:
			return ("G");
		case WEAPON_HKP2000:
			return ("E");
		case WEAPON_AK47:
			return ("W");
		case WEAPON_AWP:
			return ("Z");
		case WEAPON_BIZON:
			return ("M");
		case WEAPON_MAG7:
			return ("d");
		case WEAPON_NEGEV:
			return ("f");
		case WEAPON_SAWEDOFF:
			return ("c");
		case WEAPON_TEC9:
			return ("H");
		case WEAPON_TASER:
			return ("h");
		case WEAPON_NOVA:
			return ("e");
		case WEAPON_CZ75A:
			return ("I");
		case WEAPON_SG556:
			return ("V");
		case WEAPON_REVOLVER:
			return ("J");
		case WEAPON_MP7:
			return ("N");
		case WEAPON_MP9:
			return ("O");
		case WEAPON_MP5:
			return ("N");
		case WEAPON_C4:
			return ("o");
		case WEAPON_FRAG_GRENADE:
			return ("j");
		case WEAPON_SMOKEGRENADE:
			return ("k");
		case WEAPON_MOLOTOV:
			return ("l");
		case WEAPON_INCGRENADE:
			return ("n");
		case WEAPON_FLASHBANG:
			return ("i");
		case WEAPON_DECOY:
			return ("m");
		default:
			return ("]");
		}
		return ("");
	}
};

class C_BaseEntity : public IClientEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(int32_t, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(Vector, m_vecAngles, "DT_BaseEntity", "m_vecAngles");
	NETVAR(bool, m_bShouldGlow, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(bool, m_bSpotted, "DT_BaseEntity", "m_bSpotted");
	NETVAR(int32_t, m_nBombSite, "DT_PlantedC4", "m_nBombSite");
	NETVAR(bool, m_bBombTicking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");
	NETVAR(float, m_flTimerLength, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float, m_flDefuseLength, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float, m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
	NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
	NETVAR(int32_t, m_nSmokeEffectTickBegin, "DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin");
	NETVAR(int32_t, m_nExplodeEffectTickBegin, "DT_BaseCSGrenadeProjectile", "m_nExplodeEffectTickBegin");

	Vector GetEyesPos()
	{
		return m_vecOrigin() + m_vecViewOffset();
	}

	datamap_t* GetDataDescMap()
	{
		typedef datamap_t* (__thiscall* Fn)(void*);
		return CallVFunction<Fn>(this, 15)(this);
	}

	datamap_t* GetPredDescMap() {
		typedef datamap_t* (__thiscall* Fn)(void*);
		return CallVFunction<Fn>(this, 17)(this);
	}

	static __forceinline C_BaseEntity* GetEntityByIndex(int index) 
	{
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
	}

	static __forceinline C_BaseEntity* GetEntityFromHandle(CBaseHandle h) 
	{
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
	}

	void* Networkable() 
	{
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}

	int Index() 
	{
		typedef int (__thiscall* Fn)(void*);
		return CallVFunction<Fn>(this, 15)(this);
	}

	const matrix3x4_t& m_rgflCoordinateFrame() 
	{
		static auto m_rgflCoordinateFrame = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
		return *(matrix3x4_t*)((uintptr_t)this + m_rgflCoordinateFrame);
	}

	bool IsPlayer();
	bool IsLoot();
	bool IsWeapon();
	bool IsPlantedC4();
	bool IsDefuseKit();
};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(char, m_szCustomName, "DT_BaseAttributableItem", "m_szCustomName");
	NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_nModelIndex, "DT_BaseWeaponWorldModel", "m_nModelIndex");
	NETVAR(CHandle<C_BaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(C_EconItemView, m_Item2, "DT_BaseAttributableItem", "m_Item");
	NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");

	C_EconItemView& m_Item() 
	{
		return *(C_EconItemView*)this;
	}

	void SetModelIndex(int modelIndex);
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	NETVAR(float_t, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int32_t, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2");
	NETVAR(float_t, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(int32_t, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_iWorldDroppedModelIndex, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flPostponeFireReadyTime, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<C_BaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(int16_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_weaponMode, "DT_WeaponCSBase", "m_weaponMode");
	NETVAR(int32_t, m_zoomLevel, "DT_WeaponCSBaseGun", "m_zoomLevel");

	CCSWeaponInfo* GetCSWeaponData();
	bool HasBullets();
	bool CanFire();
	bool IsKnife();
	bool IsAwp();
	bool IsZeus();
	bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
	bool IsGun();
	bool IsGrenade();
	float GetInaccuracy();
	float GetSpread();
	void UpdateAccuracyPenalty();
	CUtlVector<IRefCounted*>& m_CustomMaterials();
	bool* m_bCustomMaterialInitialized();

	std::string weaponIconDef() {
		if (!this)
			return ("");
		int id = this->m_iItemDefinitionIndex();
		switch (id) {
		case WEAPON_KNIFE_T:
			return ("[");
		case WEAPON_DEAGLE:
			return ("A");
		case WEAPON_AUG:
			return ("U");
		case WEAPON_G3SG1:
			return ("X");
		case WEAPON_MAC10:
			return ("K");
		case WEAPON_P90:
			return ("P");
		case WEAPON_SSG08:
			return ("a");
		case WEAPON_SCAR20:
			return ("Y");
		case WEAPON_UMP45:
			return ("L");
		case WEAPON_ELITE:
			return ("B");
		case WEAPON_FAMAS:
			return ("R");
		case WEAPON_FIVESEVEN:
			return ("C");
		case WEAPON_GALILAR:
			return ("Q");
		case WEAPON_M4A1_SILENCER:
			return ("T");
		case WEAPON_M4A1:
			return ("S");
		case WEAPON_P250:
			return ("F");
		case WEAPON_M249:
			return ("g");
		case WEAPON_XM1014:
			return ("b");
		case WEAPON_GLOCK:
			return ("D");
		case WEAPON_USP_SILENCER:
			return ("G");
		case WEAPON_HKP2000:
			return ("E");
		case WEAPON_AK47:
			return ("W");
		case WEAPON_AWP:
			return ("Z");
		case WEAPON_BIZON:
			return ("M");
		case WEAPON_MAG7:
			return ("d");
		case WEAPON_NEGEV:
			return ("f");
		case WEAPON_SAWEDOFF:
			return ("c");
		case WEAPON_TEC9:
			return ("H");
		case WEAPON_TASER:
			return ("h");
		case WEAPON_NOVA:
			return ("e");
		case WEAPON_CZ75A:
			return ("I");
		case WEAPON_SG556:
			return ("V");
		case WEAPON_REVOLVER:
			return ("J");
		case WEAPON_MP7:
			return ("N");
		case WEAPON_MP9:
			return ("O");
		case WEAPON_MP5:
			return ("L");
		case WEAPON_C4:
			return ("o");
		case WEAPON_FRAG_GRENADE:
			return ("j");
		case WEAPON_SMOKEGRENADE:
			return ("k");
		case WEAPON_MOLOTOV:
			return ("l");
		case WEAPON_INCGRENADE:
			return ("n");
		case WEAPON_FLASHBANG:
			return ("i");
		case WEAPON_DECOY:
			return ("m");
		case WEAPON_KNIFE:
			return ("]");
		}
		return ("");
	}
};

class C_BasePlayer : public C_BaseEntity
{
public:
	NETVAR(bool, m_bHasDefuser, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(QAngle, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(bool, m_bHasHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "DT_CSPlayer", "m_bIsScoped");;
	NETVAR(float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, m_iHealth, "DT_BasePlayer", "m_iHealth");
	NETVAR(int32_t, m_lifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(int32_t, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int32_t, m_nTickBase, "DT_BasePlayer", "m_nTickBase");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(Vector3D, m_vecViewOffset3D, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(QAngle, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(QAngle, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(CHandle<C_BaseViewModel>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(float, m_flMaxspeed, "DT_BasePlayer", "m_flMaxspeed");
	NETVAR(CHandle<C_BasePlayer>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(int, m_iObserverMode, "DT_BasePlayer", "m_iObserverMode");
	NETVAR(float, m_flFlashMaxAlpha, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(int32_t, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(int32_t, m_iAccount, "DT_CSPlayer", "m_iAccount");
	NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(float, m_flCycle, "DT_ServerAnimationData", "m_flCycle");
	NETVAR(int32_t, m_nSequence, "DT_BaseViewModel", "m_nSequence");
	NETVAR(int32_t, m_iFOV, "DT_BasePlayer", "m_iFOV");
	NETVAR(int32_t, m_iDefaultFOV, "DT_BasePlayer", "m_iDefaultFOV");
	NETVAR(int32_t, m_nBody, "DT_BaseAnimating", "m_nBody")
	NETVAR(int32_t, m_hRagdoll, "DT_CSPlayer", "m_hRagdoll");
	NETVAR(int32_t, m_nSurvivalTeam, "DT_CSPlayer", "m_nSurvivalTeam");
	NETVAR(QAngle, m_angAbsAngles, "DT_BaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "DT_BaseEntity", "m_angAbsOrigin");
	NETVAR(float, m_flDuckSpeed, "DT_BaseEntity", "m_flDuckSpeed");
	NETVAR(float, m_flDuckAmount, "DT_BasePlayer", "m_flDuckAmount");
	PNETVAR(char, m_szLastPlaceName, "DT_BasePlayer", "m_szLastPlaceName");
	PNETVAR(CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables, "DT_BaseCombatCharacter", "m_hMyWearables");
	NETPROP(m_flLowerBodyYawTargetProp, "DT_CSPlayer", "m_flLowerBodyYawTarget");

	int C_BasePlayer::GetFOV()
	{
		if (m_iFOV() != 0)
			return m_iFOV();
		return m_iDefaultFOV();
	}

	static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(g_EngineClient->GetPlayerForUserID(id)));
	}
	static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
	}

	bool InDangerzone()
	{
		static auto game_type = g_CVar->FindVar("game_type");
		return game_type->GetInt() == 6;
	}

	bool IsEnemy()
	{
		if (InDangerzone())
		{
			return this->m_nSurvivalTeam() != g_LocalPlayer->m_nSurvivalTeam() || g_LocalPlayer->m_nSurvivalTeam() == -1;
		}
		else
		{
			return this->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();
		}
	}

	void C_BasePlayer::SetAbsAngles(const QAngle& angles)
	{
		using SetAbsAnglesFn = void(__thiscall*)(void*, const QAngle& angles);
		static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)Utils::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");

		SetAbsAngles(this, angles);
	}

	void SetAbsOriginal(Vector origin)
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector& origin);
		static SetAbsOriginFn SetAbsOrigin;
		if (!SetAbsOrigin)
		{
			SetAbsOrigin = (SetAbsOriginFn)((DWORD)Utils::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		}
		SetAbsOrigin(this, origin);
	}

	std::array<float, 24> m_flPoseParameter() const
	{
		static int _m_flPoseParameter = NetvarSys::Get().GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
	}

	template <typename T>
	T& read(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	float_t& m_surfaceFriction() 
	{
		static unsigned int _m_surfaceFriction = Utils::FindInDataMap(GetPredDescMap(), "m_surfaceFriction");
		return *(float_t*)((uintptr_t)this + _m_surfaceFriction);
	}
	Vector& m_vecBaseVelocity() 
	{
		static unsigned int _m_vecBaseVelocity = Utils::FindInDataMap(GetPredDescMap(), "m_vecBaseVelocity");
		return *(Vector*)((uintptr_t)this + _m_vecBaseVelocity);
	}

	float_t& m_flMaxspeed() 
	{
		static unsigned int _m_flMaxspeed = Utils::FindInDataMap(GetPredDescMap(), "m_flMaxspeed");
		return *(float_t*)((uintptr_t)this + _m_flMaxspeed);
	}

	PlayerInfo_t GetPlayerInfo();
	bool IsAlive();
	bool IsFlashed();
	bool HasC4();
	Vector GetHitboxPos(int hitbox_id);
	CUserCmd*& m_pCurrentCommand();
	int GetNumAnimOverlays();
	AnimationLayer* GetAnimOverlays();
	AnimationLayer* GetAnimOverlay(int i);
	int GetSequenceActivity(int sequence);
	CCSGOPlayerAnimState* GetPlayerAnimState();
	mstudiobbox_t* GetHitbox(int hitbox_id);
	bool          GetHitboxPos(int hitbox, Vector& output);
	void UpdateClientSideAnimation();
	int Handle();
	int m_nMoveType();
	float_t m_flSpawnTime();
	bool IsNotTarget();
	std::string GetName(bool console_safe = false);
	void SetModelIndex(int index);
};

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR( int32_t, m_nIndex, "DT_BaseViewModel", "m_nModelIndex" );
	NETVAR( int, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex" );
	NETVAR( int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex" );
	NETVAR(int32_t, m_hViewModel, "DT_BasePlayer", "m_hViewModel");
	NETVAR( CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon" );
	NETVAR( CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons" );
	NETVAR( CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner" );
	NETVAR( int, m_hOwner2, "DT_BaseViewModel", "m_hOwner")
	NETPROP( m_nSequence, "DT_BaseViewModel", "m_nSequence" );

	void SetModelIndex(const int index)
	{
		return CallVFunction<void(__thiscall*)(C_BaseEntity*, int)>(this, 75)(this, index);
	}

	void SendViewModelMatchingSequence( int sequence );
};

class AnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; 
	uint32_t m_nSequence; 
	float_t m_flPrevCycle; 
	float_t m_flWeight; 
	float_t m_flWeightDeltaRate;
	float_t m_flPlaybackRate; 
	float_t m_flCycle; 
	void *m_pOwner; 
	char  pad_0038[4]; 
};

class CCSGOPlayerAnimState
{
public:
	void* pThis;
	char pad2[91];
	void* pBaseEntity;
	void* pActiveWeapon; 
	void* pLastActiveWeapon;
	float m_flLastClientSideAnimationUpdateTime;
	int m_iLastClientSideAnimationUpdateFramecount; 
	float m_flEyePitch; 
	float m_flEyeYaw; 
	float m_flPitch; 
	float m_flGoalFeetYaw; 
	float m_flCurrentFeetYaw;
	float m_flCurrentTorsoYaw; 
	float m_flUnknownVelocityLean;
	float m_flLeanAmount; 
	char pad4[4]; 
	float m_flFeetCycle; 
	float m_flFeetYawRate; 
	float m_fUnknown2;
	float m_fDuckAmount; 
	float m_fLandingDuckAdditiveSomething; 
	float m_fUnknown3;
	Vector m_vOrigin; 
	Vector m_vLastOrigin; 
	float m_vVelocityX; 
	float m_vVelocityY; 
	char pad5[4];
	float m_flUnknownFloat1; 
	char pad6[8];
	float m_flUnknownFloat2; 
	float m_flUnknownFloat3; 
	float m_unknown; 
	float speed_2d; 
	float flUpVelocity; 
	float m_flSpeedNormalized; 
	float m_flFeetSpeedForwardsOrSideWays; 
	float m_flFeetSpeedUnknownForwardOrSideways; 
	float m_flTimeSinceStartedMoving; 
	float m_flTimeSinceStoppedMoving; 
	unsigned char m_bOnGround; 
	unsigned char m_bInHitGroundAnimation;
	char pad7[10];
	float m_flLastOriginZ;
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; 
	float m_flStopToFullRunningFraction; 
	char pad8[4]; 
	float m_flUnknownFraction; 
	char pad9[4]; 
	float m_flUnknown3;
	char pad10[528];
}; 

class DT_CSPlayerResource
{
public:
	PNETVAR( int32_t, m_nActiveCoinRank, "DT_CSPlayerResource", "m_nActiveCoinRank" );
	PNETVAR( int32_t, m_nMusicID, "DT_CSPlayerResource", "m_nMusicID" );
	PNETVAR( int32_t, m_nPersonaDataPublicLevel, "DT_CSPlayerResource", "m_nPersonaDataPublicLevel" );
	PNETVAR( int32_t, m_nPersonaDataPublicCommendsLeader, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsLeader" );
	PNETVAR( int32_t, m_nPersonaDataPublicCommendsTeacher, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsTeacher" );
	PNETVAR( int32_t, m_nPersonaDataPublicCommendsFriendly, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsFriendly" );
	PNETVAR( int32_t, m_iCompetitiveRanking, "DT_CSPlayerResource", "m_iCompetitiveRanking" );
	PNETVAR( int32_t, m_iCompetitiveWins, "DT_CSPlayerResource", "m_iCompetitiveWins" );
	PNETVAR( int32_t, m_iPlayerVIP, "DT_CSPlayerResource", "m_iPlayerVIP" );
	PNETVAR( int32_t, m_iMVPs, "DT_CSPlayerResource", "m_iMVPs" );
	PNETVAR( int32_t, m_iScore, "DT_CSPlayerResource", "m_iScore" );
};

class CGameRules {
public:
	NETVAR( int, m_iMatchStats_PlayersAlive_T, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_T" );
	NETVAR( int, m_iMatchStats_PlayersAlive_CT, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_CT" );
	NETVAR( int, m_iRoundTime, "DT_CSGameRulesProxy", "m_iRoundTime" );
	NETVAR( bool, m_bFreezePeriod, "DT_CSGameRulesProxy", "m_bBombDropped" );
	NETVAR( bool, m_bIsValveDS, "DT_CSGameRulesProxy", "m_bIsValveDS" );
	NETVAR( bool, m_bBombDropped, "DT_CSGameRulesProxy", "m_bBombDropped" );
	NETVAR( bool, m_bBombPlanted, "DT_CSGameRulesProxy", "m_bBombPlanted" );
	NETVAR( float, m_flSurvivalStartTime, "DT_CSGameRulesProxy", "m_flSurvivalStartTime" );
}; extern CGameRules* g_GameRules;
