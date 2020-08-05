#include "Movement.hpp"
#include "../Helpers/Configs.hpp"

void Movement::BunnyHop(CUserCmd* cmd)
{
	if (!g_Configs.misc.bunnyHop)
		return;

	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND) && cmd->buttons & IN_JUMP)
	{
		if (!(g_LocalPlayer->m_fFlags() & FL_INWATER))
		{
			int moveType = g_LocalPlayer->m_nMoveType();
			if (!(moveType == MOVETYPE_NOCLIP || moveType == MOVETYPE_LADDER))
				cmd->buttons &= ~IN_JUMP;
		}
	}
}


Movement g_Movement;