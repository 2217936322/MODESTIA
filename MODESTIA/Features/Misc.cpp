#include "Misc.hpp"

#include "../Helpers/Configs/Configs.hpp"

void Misc::RankReveal(CUserCmd* cmd)
{
	if (!g_Configs.misc.rankReveal || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	if (cmd->m_iButtons & IN_SCORE)
		g_CHLClient->DispatchUserMessage(50, 0, 0, nullptr);
}

void Misc::ClantagChanger()
{
	if (!g_EngineClient->IsConnected() && !g_EngineClient->IsInGame())
		return;

	static bool restore = false;
	static float lastChangeTime = 0.f;

	if (g_Configs.misc.clantagChanger && g_LocalPlayer) 
	{
		if (g_GlobalVars->m_fRealtime - lastChangeTime >= 0.5f)
		{
			static std::string text = "MODE$TIA ";

			lastChangeTime = g_GlobalVars->m_fRealtime;

			std::string temp = text;
			text.erase(0, 1);
			text += temp[0];

			Utils::SetClantag(text.data());
			restore = true;
		}
	}
	else if (restore) 
	{
		restore = false;
		Utils::SetClantag("");
	}
}