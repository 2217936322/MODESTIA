﻿#include <chrono>
#include <filesystem>

#include "Menu.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "../Helpers/Configs.hpp"
#include "../Helpers/ConfigSys.hpp"
#include "../Helpers/KitParser.hpp"
#include "../Helpers/ItemsDefs.hpp"
#include "../Features/SkinChanger.hpp"

C_Menu Menu;

IDirect3DStateBlock9* stateBlock;

void C_Menu::Run()
{
	static int page = 0;

	if (Opened)
	{
		ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(167 / 255.f, 24 / 255.f, 71 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(167 / 255.f, 24 / 255.f, 71 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(167 / 255.f, 24 / 71, 247 / 255.f, 1.f);

		ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("MODE$TIA", &Opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		{
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImColor c = ImColor(32, 114, 247);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 30), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 3), ImColor(30, 30, 39));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 32), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + +30), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));
			ImGui::PushFont(FontMenu);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
			ImGui::Text("MODE$TIA");
			ImGui::SameLine();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);
			if (ImGui::ButtonT("", ImVec2(40, 30), page, 0, false, false)) page = 0; ImGui::SameLine(0, 0);
			ImGui::PopFont();

			ImGui::PushFont(FontMenu);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 45);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 222);

			if (k_Skins.size() == 0)
			{
				InitializeKits();
			}

			auto& entries = g_Configs.skins.m_Items;
			static auto definitionVectorIndex = 0;

			const auto wnd = ImGui::GetCurrentWindowRead();
			float height = (wnd->Pos.y + wnd->Size.y) - wnd->DC.CursorPos.y - 18.0f - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().FramePadding.y * 2.0f;

			switch (page)
			{
			case 0:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("Weapon", ImVec2(279, 211), true);
				{
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 18);
					ImGui::ListBoxHeader("##weapons");
					{
						for (size_t w = 0; w < k_WeaponNames.size(); w++)
						{
							if (ImGui::Selectable(k_WeaponNames[w].name, definitionVectorIndex == w))
							{
								definitionVectorIndex = w;
							}
						}
					}
					ImGui::ListBoxFooter();

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
					if (ImGui::Button("Update"))
					{
						Skins::ScheduleHUDUpdate();
					}
				} ImGui::EndChild(true);


				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16);
				ImGui::BeginChild("Misc", ImVec2(279, 324), true);
				{
					ImGui::Checkbox("Bunny Hop", &g_Configs.misc.bunnyHop);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
					ImGui::Combo("Keybinds", &g_Configs.misc.keyBindSelection, "EdgeJump\0Menu Key");
					if (g_Configs.misc.keyBindSelection == 0) 
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 12);
						ImGui::Hotkey("##edgeJumpKey", &g_Configs.misc.edgeJumpKey, ImVec2(100, 20));
					}
					else if (g_Configs.misc.keyBindSelection == 1) 
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 12);
						ImGui::Hotkey("##menuKey", &g_Configs.misc.menuKey, ImVec2(100, 20));
					}

					constexpr auto playerModels = "Default\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0Pirate\0Pirate Variant A\0Pirate Variant B\0Pirate Variant C\0Pirate Variant D\0Anarchist\0Anarchist Variant A\0Anarchist Variant B\0Anarchist Variant C\0Anarchist Variant D\0Balkan Variant A\0Balkan Variant B\0Balkan Variant C\0Balkan Variant D\0Balkan Variant E\0Jumpsuit Variant A\0Jumpsuit Variant B\0Jumpsuit Variant C\0";
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
					ImGui::Combo("TR Model", &g_Configs.misc.playerModelT, playerModels);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
					ImGui::Combo("CT Model", &g_Configs.misc.playerModelCT, playerModels);

					constexpr auto knifeModels = "Default\0Minecraft Pickaxe\0Fidget Spinner\0";
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
					ImGui::Combo("Knife Model", &g_Configs.misc.knifeModel, knifeModels);

					constexpr auto awpModels = "Default\0DSR-50\0";
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
					ImGui::Combo("AWP Model", &g_Configs.misc.awpModel, awpModels);

				} ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::NextColumn();


				ImGui::BeginChild("Skins", ImVec2(279, 202), true);
				{
					auto& selectedEntry = entries[k_WeaponNames[definitionVectorIndex].definitionIndex];
					selectedEntry.definitionIndex = k_WeaponNames[definitionVectorIndex].definitionIndex;
					selectedEntry.definitionVectorIndex = definitionVectorIndex;
					ImGui::Checkbox("Enabled", &selectedEntry.enabled);
					ImGui::InputText("Name Tag", selectedEntry.customName, 32);
					ImGui::InputInt("Pattern", &selectedEntry.seed);
					ImGui::InputInt("StatTrak\u2122", &selectedEntry.stattrak);
					ImGui::SliderFloat("Float", &selectedEntry.wear, FLT_MIN, 1.f, "%.10f", 5);
					if (selectedEntry.definitionIndex != GLOVE_T_SIDE)
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
						ImGui::Combo("Paint Kit", &selectedEntry.paintKitVectorIndex, [](void* data, int idx, const char** out_text)
							{
								*out_text = k_Skins[idx].name.c_str();
								return true;
							}, nullptr, k_Skins.size(), 20);
						selectedEntry.paintKitIndex = k_Skins[selectedEntry.paintKitVectorIndex].id;
					}
					else
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
						ImGui::Combo("Paint Kit", &selectedEntry.paintKitVectorIndex, [](void* data, int idx, const char** out_text)
							{
								*out_text = k_Gloves[idx].name.c_str();
								return true;
							}, nullptr, k_Gloves.size(), 20);
						selectedEntry.paintKitIndex = k_Gloves[selectedEntry.paintKitVectorIndex].id;
					}
					if (selectedEntry.definitionIndex == WEAPON_KNIFE)
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
						ImGui::Combo("Knife", &selectedEntry.definitionOverrideVectorIndex, [](void* data, int idx, const char** out_text)
							{
								*out_text = k_KnifeNames.at(idx).name;
								return true;
							}, nullptr, k_KnifeNames.size(), 10);
						selectedEntry.definitionOverrideIndex = k_KnifeNames.at(selectedEntry.definitionOverrideVectorIndex).definitionIndex;
					}
					else if (selectedEntry.definitionIndex == GLOVE_T_SIDE)
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
						ImGui::Combo("Glove", &selectedEntry.definitionOverrideVectorIndex, [](void* data, int idx, const char** out_text) {
							*out_text = k_GloveNames.at(idx).name;
							return true;
							}, nullptr, k_GloveNames.size(), 10);
						selectedEntry.definitionOverrideIndex = k_GloveNames.at(selectedEntry.definitionOverrideVectorIndex).definitionIndex;
					}
					else
					{
						static auto unusedValue = 0;
						selectedEntry.definitionOverrideVectorIndex = 0;
					}
				}  ImGui::EndChild(true);

				ImGui::BeginChild("Configs", ImVec2(279, 333), true);
				{

					static std::vector<std::string> configs;

					static auto loadConfigs = []()
					{
						std::vector<std::string> items = {};

						std::string path("C:\\MODE$TIA");
						if (!std::filesystem::is_directory(path))
							std::filesystem::create_directories(path);

						for (auto& p : std::filesystem::directory_iterator(path))
							items.push_back(p.path().string().substr(path.length() + 1));

						return items;
					};

					static auto isConfigLoaded = false;
					if (!isConfigLoaded) 
					{
						isConfigLoaded = true;
						configs = loadConfigs();
					}

					static std::string currentConfig;
					static char configName[32];

					ImGui::InputText("##configName", configName, sizeof(configName));
					ImGui::SameLine();
					if (ImGui::Button("Create"))
					{
						currentConfig = std::string(configName);

						Config->Save(currentConfig + ".ini");
						isConfigLoaded = false;
						memset(configName, 0, 32);
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 18);
					ImGui::ListBoxHeader("##configs");
					{
						for (auto& config : configs) {
							if (ImGui::Selectable(config.c_str(), config == currentConfig)) {
								currentConfig = config;
							}
						}
					}
					ImGui::ListBoxFooter();


					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
					if (!currentConfig.empty())
					{

						if (ImGui::Button("Load"))
							Config->Load(currentConfig);
						ImGui::SameLine();

						if (ImGui::Button("Save"))
							Config->Save(currentConfig);

						ImGui::SameLine();

						if (ImGui::Button("Delete") && std::filesystem::remove("C:\\MODE$TIA\\" + currentConfig)) 
						{
							currentConfig.clear();
							isConfigLoaded = false;
						}

						ImGui::SameLine();
					}
					if (ImGui::Button("Refresh"))
						isConfigLoaded = false;

				} ImGui::EndChild(true);


				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			}
		}

		ImGui::PopFont();

		ImGui::End();
	}
}