#include "Movement.hpp"

#include "EnginePrediction.hpp"

#include "../Helpers/InputSystem.hpp"
#include "../Helpers/Configs/Configs.hpp"

void Movement::EdgeJump(CUserCmd* cmd)
{
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	int moveType = g_LocalPlayer->m_nMoveType();
	if (moveType == MOVETYPE_LADDER || moveType == MOVETYPE_NOCLIP)
		return;

	if (InputSystem::Get().IsKeyDown(g_Configs.misc.edgeJumpKey) && (EnginePrediction::GetFlags() & FL_ONGROUND) && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		cmd->m_iButtons |= IN_JUMP;

	if (InputSystem::Get().IsKeyDown(g_Configs.misc.edgeJumpKey) && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		cmd->m_iButtons |= IN_DUCK;
}