#pragma once

#include "../SDK/SDK.hpp"
#include "../SDK/CSGOStructs.hpp"

class Misc : public Singleton<Misc>
{
	friend class Singleton<Misc>;

public:
	void RankReveal(CUserCmd* cmd);
	void ClantagChanger();
};