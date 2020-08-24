#pragma once

#include <string>
#include <map>

#include "../SDK/CSGOStructs.hpp"

#undef min

struct CItemSettings
{
	bool enabled = false;
	int definitionVectorIndex = 0;
	int definitionIndex = 1;
	int paintKitVectorIndex = 0;
	int paintKitIndex = 0;
	int definitionOverrideVectorIndex = 0;
	int definitionOverrideIndex = 0;
	int seed = 0;
	int stattrak = -1;
	float wear = std::numeric_limits<float>::min();
	char customName[32] = "";
};

class Configs
{
public:
	struct
	{
		bool bunnyHop = false;
		bool autoAccept = false;
		bool rankReveal = false;
		bool clantagChanger = false;

		int keyBindSelection = 0;
		int edgeJumpKey = 0;
		int menuKey = 45;
	} misc;

	struct
	{
		int playerModelT = 0;
		int playerModelCT = 0;
		int knifeModel = 0;
		int awpModel = 0;
	} modelChanger;

	struct
	{
		std::map<int, CItemSettings> m_Items;
		std::unordered_map<std::string, std::string> m_IconOverrides;
		const char* GetIconOverride(const std::string original)
		{
			return m_IconOverrides.count(original) ? m_IconOverrides.at(original).data() : nullptr;
		}
	} skinChanger;
};

extern Configs g_Configs;