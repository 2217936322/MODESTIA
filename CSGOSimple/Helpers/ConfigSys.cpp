#include "ConfigSys.hpp"

#include "Configs.hpp"
#include "KitParser.hpp"
#include "ItemsDefs.hpp"
#include "Utils.hpp"
#include "../Features/SkinChanger.hpp"

CConfig* Config = new CConfig();

void CConfig::SetupValue(int& value, int def, std::string category, std::string name) { value = def; ints.push_back(new ConfigValue< int >(category, name, &value, def)); }
void CConfig::SetupValue(float& value, float def, std::string category, std::string name) { value = def; floats.push_back(new ConfigValue< float >(category, name, &value, def)); }
void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name) { value = def; bools.push_back(new ConfigValue< bool >(category, name, &value, def)); }
void CConfig::SetupValue(char* value, char* def, std::string category, std::string name) { value = def; chars.push_back(new ConfigValue< char >(category, name, value, *def)); }

void CConfig::SetupMisc()
{
	SetupValue(g_Configs.misc.autoAccept, false, ("Misc"), ("autoAccept"));
	SetupValue(g_Configs.misc.rankReveal, false, ("Misc"), ("rankReveal"));
	SetupValue(g_Configs.misc.keyBindSelection, 0, ("Misc"), ("keyBindSelection"));
	SetupValue(g_Configs.misc.edgeJumpKey, 0, ("Misc"), ("edgeJumpKey"));
	SetupValue(g_Configs.misc.menuKey, 45, ("Misc"), ("menuKey"));
}

void CConfig::SetupModelChanger()
{
	SetupValue(g_Configs.modelChanger.playerModelT, 0, ("ModelChanger"), ("playerModelT"));
	SetupValue(g_Configs.modelChanger.playerModelCT, 0, ("ModelChanger"), ("playerModelCT"));
}

void CConfig::SetupSkinChanger()
{
	for (auto& key : k_WeaponNames)
	{
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].enabled, false, (key.name), ("enabled"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].definitionVectorIndex, 0, (key.name), ("definitionVectorIndex"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].customName, "", (key.name), ("customName"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].definitionIndex, 1, (key.name), ("definitionIndex"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].paintKitVectorIndex, 0, (key.name), ("paintKitVectorIndex"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].paintKitIndex, 0, (key.name), ("paintKitIndex"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].definitionOverrideVectorIndex, 0, (key.name), ("definitionOverrideVectorIndex"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].definitionOverrideIndex, 0, (key.name), ("definitionOverrideIndex"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].seed, 0, (key.name), ("pattern"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].stattrak, -1, (key.name), ("stattrak"));
		SetupValue(g_Configs.skinChanger.m_Items[key.definitionIndex].wear, 0.0f, (key.name), ("float"));
	}
}

void CConfig::Setup()
{
	CConfig::SetupMisc();
	CConfig::SetupModelChanger();
	CConfig::SetupSkinChanger();
}

void CConfig::Save(const std::string& name)
{
	if (name.empty())
		return;

	CreateDirectoryA(u8"C:\\MODE$TIA\\", NULL);
	std::string file = u8"C:\\MODE$TIA\\" + name;

	for (auto value : ints)
	{
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	}

	for (auto value : floats)
	{
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	}

	for (auto value : bools)
	{
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
	}

	for (auto value : chars)
	{
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), value->value, file.c_str());
	}
}

void CConfig::Load(const std::string& name)
{
	if (name.empty())
		return;

	SkinChanger::Get().ScheduleHUDUpdate();

	CreateDirectoryA(u8"C:\\MODE$TIA\\", NULL);
	std::string file = u8"C:\\MODE$TIA\\" + name;

	char value_l[32] = { '\0' };
	for (auto value : ints)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0", value_l, 32, file.c_str()); *value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0.0f", value_l, 32, file.c_str()); *value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "false", value_l, 32, file.c_str()); *value->value = !strcmp(value_l, "true");
	}
}
