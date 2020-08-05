#include "CSGOStructs.hpp"
#include "Math/Math.hpp"
#include "../Helpers/Utils.hpp"

CGameRules* g_GameRules = nullptr;

bool C_BaseEntity::IsPlayer()
{
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 159)(this);
}

int C_BasePlayer::Handle()
{
	return CallVFunction<int(__thiscall*)(C_BasePlayer*)>(this, 2)(this);
}

bool C_BaseEntity::IsLoot()
{
	return (GetClientClass()->m_ClassID == CPhysPropAmmoBox ||
		GetClientClass()->m_ClassID == CPhysPropLootCrate ||
		GetClientClass()->m_ClassID == CPhysPropRadarJammer ||
		GetClientClass()->m_ClassID == CPhysPropWeaponUpgrade ||
		GetClientClass()->m_ClassID == CDrone ||
		GetClientClass()->m_ClassID == CDronegun ||
		GetClientClass()->m_ClassID == CItem_Healthshot ||
		GetClientClass()->m_ClassID == CItemCash);
}

bool C_BaseEntity::IsWeapon()
{
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 169)(this);
}

bool C_BaseEntity::IsPlantedC4() 
{
	return GetClientClass()->m_ClassID == CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit( ) 
{
	return GetClientClass( )->m_ClassID == CBaseAnimating;
}

CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData() 
{
	return CallVFunction<CCSWeaponInfo* (__thiscall*)(C_BaseCombatWeapon*)>(this, 460)(this);
}

bool C_BaseCombatWeapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::CanFire()
{
	static decltype(this) stored_weapon = nullptr;
	static auto stored_tick = 0;
	if (stored_weapon != this || stored_tick >= g_LocalPlayer->m_nTickBase()) {
		stored_weapon = this;
		stored_tick = g_LocalPlayer->m_nTickBase();
		return false; //cannot shoot first tick after switch
	}

	if (IsReloading() || m_iClip1() <= 0 || !g_LocalPlayer)
		return false;

	auto flServerTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

	return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::IsGun()
{
	switch (GetCSWeaponData()->WeaponType) {
	case WEAPONTYPE_C4:
		return false;
	case WEAPONTYPE_GRENADE:
		return false;
	case WEAPONTYPE_KNIFE:
		return false;
	case WEAPONTYPE_UNKNOWN:
		return false;
	default:
		return true;
	}
}

bool C_BaseCombatWeapon::IsGrenade() 
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_GRENADE;
}

bool C_BaseCombatWeapon::IsKnife()
{
	if (this->m_Item().m_iItemDefinitionIndex() == WEAPON_TASER) return false;
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_KNIFE;
}

bool C_BaseCombatWeapon::IsAwp()
{
	if (this->m_Item().m_iItemDefinitionIndex() == WEAPON_AWP)
		return true;
	return false;
}

bool C_BaseCombatWeapon::IsZeus() 
{
	if (this->m_Item().m_iItemDefinitionIndex() == WEAPON_TASER)
		return true;
	return false;
}

bool C_BaseCombatWeapon::IsRifle()
{
	switch (GetCSWeaponData()->WeaponType)
	{
	case WEAPONTYPE_RIFLE:
		return true;
	case WEAPONTYPE_SUBMACHINEGUN:
		return true;
	case WEAPONTYPE_SHOTGUN:
		return true;
	case WEAPONTYPE_MACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsPistol() 
{
	switch (GetCSWeaponData()->WeaponType) 
	{
	case WEAPONTYPE_PISTOL:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsSniper()
{
	switch (GetCSWeaponData()->WeaponType)
	{
	case WEAPONTYPE_SNIPER_RIFLE:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsReloading()
{
	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
	return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 482)(this);
}

float C_BaseCombatWeapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 452)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 483)(this);
}

CUtlVector<IRefCounted*>& C_BaseCombatWeapon::m_CustomMaterials()
{
	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "83 BE ? ? ? ? ? 7F 67") + 2) - 12;
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + inReload);
}

bool* C_BaseCombatWeapon::m_bCustomMaterialInitialized()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "C6 86 ? ? ? ? ? FF 50 04") + 2);
	return (bool*)((uintptr_t)this + currentCommand);
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

int C_BasePlayer::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x298C);
}

AnimationLayer* C_BasePlayer::GetAnimOverlays()
{
	return *(AnimationLayer**)((DWORD)this + 0x2980);
}

AnimationLayer* C_BasePlayer::GetAnimOverlay(int i) 
{
	if (i < 15)
		return &GetAnimOverlays()[i];
	return nullptr;
}

int C_BasePlayer::GetSequenceActivity(int sequence)
{
	auto hdr = g_MdlInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto getSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::PatternScan(GetModuleHandle(L"client.dll"), "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));

	return getSequenceActivity(this, hdr, sequence);
}

CCSGOPlayerAnimState* C_BasePlayer::GetPlayerAnimState()
{
	return *(CCSGOPlayerAnimState**)((DWORD)this + 0x3900);
}


PlayerInfo_t C_BasePlayer::GetPlayerInfo()
{
	PlayerInfo_t info;
	g_EngineClient->GetPlayerInfo(EntIndex(), &info);
	return info;
}

bool C_BasePlayer::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

bool C_BasePlayer::IsFlashed()
{
	static auto m_flFlashMaxAlpha = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	return *(float*)((uintptr_t)this + m_flFlashMaxAlpha - 0x8) > 200.0;
}

bool C_BasePlayer::HasC4()
{
	static auto fnHasC4
		= reinterpret_cast<bool(__thiscall*)(void*)>(
			Utils::PatternScan(GetModuleHandleW(L"client.dll"), "56 8B F1 85 F6 74 31")
			);

	return fnHasC4(this);
}

Vector C_BasePlayer::GetHitboxPos(int hitboxID)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
	{
		auto studioModel = g_MdlInfo->GetStudiomodel(GetModel());
		if (studioModel)
		{
			auto hitbox = studioModel->GetHitboxSet(0)->GetHitbox(hitboxID);
			if (hitbox)
			{
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

mstudiobbox_t* C_BasePlayer::GetHitbox(int hitboxID)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
	{
		auto studioModel = g_MdlInfo->GetStudiomodel(GetModel());
		if (studioModel)
		{
			auto hitbox = studioModel->GetHitboxSet(0)->GetHitbox(hitboxID);
			if (hitbox)
			{
				return hitbox;
			}
		}
	}
	return nullptr;
}

bool C_BasePlayer::GetHitboxPos(int hitbox, Vector& output)
{
	if (hitbox >= HITBOX_MAX)
		return false;

	const model_t* model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t* studioHdr = g_MdlInfo->GetStudiomodel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
		return false;

	mstudiobbox_t* studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}

std::string C_BasePlayer::GetName(bool console_safe)
{
	PlayerInfo_t pinfo = this->GetPlayerInfo();

	char* pl_name = pinfo.name;
	char buf[128];
	int c = 0;

	for (int i = 0; pl_name[i]; ++i)
	{
		if (c >= sizeof(buf) - 1)
			break;

		switch (pl_name[i])
		{
		case '"': if (console_safe) break;
		case '\\':
		case ';': if (console_safe) break;
		case '\n':
			break;
		default:
			buf[c++] = pl_name[i];
		}
	}

	buf[c] = '\0';
	return std::string(buf);
}

void C_BasePlayer::UpdateClientSideAnimation()
{
	return CallVFunction< void(__thiscall*)(void*) >(this, 223)(this);
}

int C_BasePlayer::m_nMoveType()
{
	return *(int*)((uintptr_t)this + 0x25c);
}

void C_BasePlayer::SetModelIndex(const int index)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, index);
}


void C_BaseAttributableItem::SetModelIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

void C_BaseViewModel::SendViewModelMatchingSequence(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 246)(this, sequence);
}

float_t C_BasePlayer::m_flSpawnTime()
{
	return *(float_t*)((uintptr_t)this + 0xA360);
}

bool C_BasePlayer::IsNotTarget()
{
	if (!this || this == g_LocalPlayer)
		return true;

	if (m_iHealth() <= 0)
		return true;

	if (m_bGunGameImmunity())
		return true;

	if (m_fFlags() & FL_FROZEN)
		return true;

	int entIndex = EntIndex();
	return entIndex > g_GlobalVars->maxClients;
}
