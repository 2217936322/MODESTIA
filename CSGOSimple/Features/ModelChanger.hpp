#pragma once

#include "../SDK/CSGOStructs.hpp"

class ModelChanger : public Singleton<ModelChanger>
{
	friend class Singleton<ModelChanger>;

public:
	void PlayerChanger(ClientFrameStage_t stage);
};
