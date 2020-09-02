#pragma once

#include "../SDK/CSGOStructs.hpp"

class SkinChanger : public Singleton<SkinChanger>
{
	friend class Singleton<SkinChanger>;

public:
	void Run(ClientFrameStage_t stage);
	void ScheduleHUDUpdate();
};

