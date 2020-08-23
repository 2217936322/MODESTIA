#include "Misc.hpp"

#include "../Helpers/Configs.hpp"

void Misc::RankReveal(CUserCmd* cmd)
{
	if (!g_Configs.misc.rankReveal || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	if (cmd->buttons & IN_SCORE)
		g_CHLClient->DispatchUserMessage(50, 0, 0, nullptr);
}