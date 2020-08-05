#pragma once

#include "../SDK/SDK.hpp"
#include "../SDK/CSGOStructs.hpp"

class Movement 
{
public: 
	void BunnyHop(CUserCmd* cmd);
};

extern Movement g_Movement;