#include "SkinChanger.hpp"
#include "../Helpers/ItemsDefs.hpp"
#include "../Helpers/KitParser.hpp"
#include "../Helpers/Configs.hpp"
#include "../Helpers/Memory.hpp"

static void EraseOverrideIfExistsByIndex(const int definitionIndex)
{
	if (k_WeaponInfo.count(definitionIndex))
	{
		auto& IconOverrideMap = g_Configs.skins.m_IconOverrides;
		const auto& OriginalItem = k_WeaponInfo.at(definitionIndex);
		if (OriginalItem.icon && IconOverrideMap.count(OriginalItem.icon))
			IconOverrideMap.erase(IconOverrideMap.at(OriginalItem.icon));
	}
}

static void ApplyConfigOnAttributableItem(CBaseAttributableItem* item, const C_ItemSettings* config, const unsigned xuidLow)
{
	if (!config->enabled)
	{
		return;
	}

	item->m_Item().m_iItemIDHigh() = -1;

	item->m_Item().m_iAccountID() = xuidLow;

	if (config->customName[0])
		strcpy_s(item->m_Item().m_szCustomName(), config->customName);

	if (config->paintKitIndex)
		item->m_nFallbackPaintKit() = config->paintKitIndex;

	if (config->seed)
		item->m_nFallbackSeed() = config->seed;

	if (config->stattrak > -1)
	{
		item->m_nFallbackStatTrak() = config->stattrak;
		item->m_Item().m_iEntityQuality() = 9;
	}
	else
	{
		item->m_Item().m_iEntityQuality() = IsKnife(config->definitionIndex) ? 3 : 0;
	}

	item->m_flFallbackWear() = config->wear;

	auto& definitionIndex = item->m_Item().m_iItemDefinitionIndex();

	auto& IconOverrideMap = g_Configs.skins.m_IconOverrides;
	if (config->definitionOverrideIndex && config->definitionOverrideIndex != definitionIndex && k_WeaponInfo.count(config->definitionOverrideIndex))
	{
		const auto oldDefinitionIndex = definitionIndex;
		definitionIndex = config->definitionOverrideIndex;
		const auto& replacementItem = k_WeaponInfo.at(config->definitionOverrideIndex);
		item->m_nModelIndex() = g_MdlInfo->GetModelIndex(replacementItem.model);
		item->SetModelIndex(g_MdlInfo->GetModelIndex(replacementItem.model));
		item->GetClientNetworkable()->PreDataUpdate(0);
		if (oldDefinitionIndex && k_WeaponInfo.count(oldDefinitionIndex))
		{
			const auto& OriginalItem = k_WeaponInfo.at(oldDefinitionIndex);
			if (OriginalItem.icon && replacementItem.icon)
			{
				IconOverrideMap[OriginalItem.icon] = replacementItem.icon;
			}
		}
	}
	else
	{
		EraseOverrideIfExistsByIndex(definitionIndex);
	}
}

static CBaseEntity* MakeGlove(int entry, int serial)
{
	static std::add_pointer_t<CBaseEntity* __cdecl(int, int)> createWearable = nullptr;

	if (!createWearable) {
		createWearable = []() -> decltype(createWearable) {
			for (auto clientClass = g_CHLClient->GetAllClasses(); clientClass; clientClass = clientClass->m_pNext)
				if (clientClass->m_ClassID == C_EconWearable)
					return clientClass->createFunction;
			return nullptr;
		}();
	}

	if (!createWearable)
		return nullptr;

	createWearable(entry, serial);
	return g_EntityList->GetClientEntity(entry);
}

void Initialize(int localHandle)
{
	const auto localIndex = g_EngineClient->GetLocalPlayer();
	const auto local = static_cast<CBasePlayer*>(g_EntityList->GetClientEntity(localIndex));
	if (!local)
		return;

	PlayerInfo_t playerInfo;
	if (!g_EngineClient->GetPlayerInfo(localIndex, &playerInfo))
		return;

	{
		const auto wearables = local->m_hMyWearables();
		const auto gloveConfig = &g_Configs.skins.m_Items[GLOVE_T_SIDE];

		static auto gloveHandle = CBaseHandle(0);
		auto glove = reinterpret_cast<CBaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(wearables[0]));

		if (!glove)
		{
			const auto ourGlove = reinterpret_cast<CBaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(gloveHandle));
			if (ourGlove)
			{
				wearables[0] = gloveHandle;
				glove = ourGlove;
			}
		}

		if (!local->IsAlive())
		{
			if (glove)
			{
				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
				glove->GetClientNetworkable()->Release();
			}
			return;
		}

		if (gloveConfig && gloveConfig->definitionOverrideIndex)
		{
			if (!glove)
			{
				auto entry = g_EntityList->GetHighestEntityIndex() + 1;

				for (int i = 65; i <= g_EntityList->GetHighestEntityIndex(); i++) {
					auto entity = g_EntityList->GetClientEntity(i);

					if (entity && entity->GetClientClass()->m_ClassID == ClassID(70)) {
						entry = i;
						break;
					}
				}

				const auto serial = rand() % 0x1000;

				glove = reinterpret_cast<CBaseAttributableItem*>(MakeGlove(entry, serial));
				if (glove)
				{
					glove->m_bInitialized() = true;

					const auto wearableHandle = reinterpret_cast<CBaseHandle*>(&wearables[0]);
					*wearableHandle = entry | serial << 16;
					gloveHandle = wearables[0];
				}
			}

			if (glove)
			{
				g_Memory.EquipWearable(glove, local);
				local->m_nBody() = 1;

				ApplyConfigOnAttributableItem(glove, gloveConfig, playerInfo.xuidlow);
			}
		}
	}

	{
		auto weapons = local->m_hMyWeapons();
		for (int i = 0; weapons[i].IsValid(); i++)
		{
			CBaseAttributableItem* weapon = (CBaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(weapons[i]);
			if (!weapon)
				continue;

			auto& definitionIndex = weapon->m_Item().m_iItemDefinitionIndex();

			if (const auto activeConf = &g_Configs.skins.m_Items[IsKnife(definitionIndex) ? WEAPON_KNIFE : definitionIndex])
				ApplyConfigOnAttributableItem(weapon, activeConf, playerInfo.xuidlow);
			else
				EraseOverrideIfExistsByIndex(definitionIndex);
		}

		const auto viewmodelHandle = local->m_hViewModel();
		if (!viewmodelHandle.IsValid())
			return;

		const auto viewmodel = static_cast<CBaseViewModel*>(g_EntityList->GetClientEntityFromHandle(viewmodelHandle));
		if (!viewmodel)
			return;

		const auto viewmodelWeaponHandle = viewmodel->m_hWeapon();
		if (!viewmodelWeaponHandle.IsValid())
			return;

		const auto viewmodelWeapon = static_cast<CBaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(viewmodelWeaponHandle));
		if (!viewmodelWeapon)
			return;

		if (k_WeaponInfo.count(viewmodelWeapon->m_Item().m_iItemDefinitionIndex()))
		{
			const auto overrideModel = k_WeaponInfo.at(viewmodelWeapon->m_Item().m_iItemDefinitionIndex()).model;
			auto overrideModelIndex = g_MdlInfo->GetModelIndex(overrideModel);

			viewmodel->m_nModelIndex() = overrideModelIndex;

			auto worldModelHandle = viewmodelWeapon->m_hWeaponWorldModel();
			if (!worldModelHandle.IsValid())
				return;

			const auto worldModel = static_cast<CBaseWeaponWorldModel*>(g_EntityList->GetClientEntityFromHandle(worldModelHandle));
			if (!worldModel)
				return;

			worldModel->m_nModelIndex() = overrideModelIndex + 1;
		}
	}
}

static bool hudUpdateRequired = false;
static constexpr void UpdateHUD()
{
	if (auto hudWeapons = g_Memory.FindHudElement(g_Memory.Hud, "CCSGO_HudWeaponSelection") - 0x28) 
	{
		for (int i = 0; i < *(hudWeapons + 0x20); i++)
			i = g_Memory.ClearHudWeapon(hudWeapons, i);
	}

	hudUpdateRequired = false;
}

void Skins::Run(ClientFrameStage_t stage)
{
	static int localPlayerHandle = -1;

	if (g_LocalPlayer)
		localPlayerHandle = g_LocalPlayer->Handle();

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) 
	{
		Initialize(localPlayerHandle);
		if (hudUpdateRequired && g_LocalPlayer && !g_LocalPlayer->IsDormant())
			UpdateHUD();
	}
}

void Skins::ScheduleHUDUpdate()
{
	g_ClientState->ForceFullUpdate();
	hudUpdateRequired = true;
}