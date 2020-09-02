#include "CSGOStructs.hpp"

#include "Math/Math.hpp"
#include "../Helpers/Utils.hpp"

CGameRules* g_GameRules = nullptr;

void CBaseEntity::SetModelIndex(const int modelIndex)
{
	using Fn = void(__thiscall*)(CBaseEntity*, const int);
	return CallVirtualFunction<Fn>(this, 75)(this, modelIndex);
}

bool CBasePlayer::IsPlayer()
{
	using Fn = bool(__thiscall*)(CBasePlayer*);
	return CallVirtualFunction<Fn>(this, 159)(this);
}

bool CBasePlayer::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

int CBasePlayer::Handle()
{
	using Fn = int(__thiscall*)(CBasePlayer*);
	return CallVirtualFunction<Fn>(this, 2)(this);
}

int CBasePlayer::m_nMoveType()
{
	return *(int*)((uintptr_t)this + 0x25c);
}