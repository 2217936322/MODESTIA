#pragma once

#include "../SDK/SDK.hpp"
#include "../SDK/CSGOStructs.hpp"

class Movement : public Singleton<Movement>
{
	friend class Singleton<Movement>;

public:
	void EdgeJump(CUserCmd* cmd);
};
