#pragma once

#include "../SDK/SDK.hpp"
#include "../SDK/CSGOStructs.hpp"

class Movement : public Singleton<Movement>
{
	friend class Singleton<Movement>;

public:
	void BunnyHop(CUserCmd* cmd);
	void EdgeJump(CUserCmd* cmd);
};
