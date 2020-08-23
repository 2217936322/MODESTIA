#include "Movement.hpp"

#include "EnginePrediction.hpp"

#include "../Helpers/Configs.hpp"
#include "../Helpers/InputSys.hpp"

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

void Movement::EdgeJump(CUserCmd* cmd)
{
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	int moveType = g_LocalPlayer->m_nMoveType();
	if (moveType == MOVETYPE_LADDER || moveType == MOVETYPE_NOCLIP)
		return;

	if (InputSys::Get().IsKeyDown(g_Configs.misc.edgeJumpKey))
	{
		if ((EnginePrediction::GetFlags() & FL_ONGROUND) && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
			cmd->buttons |= IN_JUMP;

		if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
			cmd->buttons |= IN_DUCK;
	}
}