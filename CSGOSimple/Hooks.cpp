#include <intrin.h>

#include "Hooks.hpp"

#include "Helpers/Fnv.hpp"
#include "Helpers/Configs.hpp"
#include "Helpers/InputSys.hpp"
#include "Helpers/MinHook/minhook.h"

#include "Features/Movement.hpp"
#include "Features/SkinChanger.hpp"
#include "Features/ModelChanger.hpp"
#include "Features/EnginePrediction.hpp"
#include "Helpers/ItemsDefs.hpp"
#include "Menu/Menu.hpp"

RecvPropHook* SequenceHook;
Hooks::CreateMove::Fn CreateMoveOriginal = nullptr;
Hooks::EmitSound::Fn EmitSoundOriginal = nullptr;
Hooks::FireEvent::Fn FireEventOriginal = nullptr;
Hooks::FrameStageNotify::Fn FrameStageNotifyOriginal = nullptr;
Hooks::LockCursor::Fn LockCursorOriginal = nullptr;
Hooks::EndScene::Fn EndSceneOriginal = nullptr;
Hooks::Reset::Fn ResetOriginal = nullptr;
Hooks::CheckFileCRCsWithServer::Fn CheckFileCRCsWithServerOriginal = nullptr;

unsigned int GetVirtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }

bool Hooks::Initialize()
{
	auto CreateMoveTarget = reinterpret_cast<void*>(GetVirtual(g_ClientMode, 24));
	auto EmitSoundTarget = reinterpret_cast<void*>(GetVirtual(g_EngineSound, 5));
	auto FireEventTarget = reinterpret_cast<void*>(GetVirtual(g_GameEvents, 9));
	auto FrameStageNotifyTarget = reinterpret_cast<void*>(GetVirtual(g_CHLClient, 37));
	auto LockCursorTarget = reinterpret_cast<void*>(GetVirtual(g_VGuiSurface, 67));
	auto EndSceneTarget = reinterpret_cast<void*>(GetVirtual(g_D3DDevice9, 42));
	auto ResetTarget = reinterpret_cast<void*>(GetVirtual(g_D3DDevice9, 16));
	auto LooseFileAllowedTarget = reinterpret_cast<void*>(GetVirtual(g_FileSystem, 128));
	auto CheckFileCRCsWithServerTarget = reinterpret_cast<void*>(Utils::PatternScan(GetModuleHandleA("engine.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80"));

	if (MH_Initialize() != MH_OK)
	{
		throw std::runtime_error("Failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(CreateMoveTarget, &CreateMove::Hook, reinterpret_cast<void**>(&CreateMoveOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize CreateMove. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(EmitSoundTarget, &EmitSound::Hook, reinterpret_cast<void**>(&EmitSoundOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize EmitSound. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(FireEventTarget, &FireEvent::Hook, reinterpret_cast<void**>(&FireEventOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize FireEvent. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(FrameStageNotifyTarget, &FrameStageNotify::Hook, reinterpret_cast<void**>(&FrameStageNotifyOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize FrameStageNotify. (Outdated index?)");
		return false;
	}

	SequenceHook = new RecvPropHook(CBaseViewModel::m_nSequence(), Hooks::RecvProxy::Hook);

	if (MH_CreateHook(LockCursorTarget, &LockCursor::Hook, reinterpret_cast<void**>(&LockCursorOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize LockCursor. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(EndSceneTarget, &EndScene::Hook, reinterpret_cast<void**>(&EndSceneOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize EndScene. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(ResetTarget, &Reset::Hook, reinterpret_cast<void**>(&ResetOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize Reset. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(LooseFileAllowedTarget, &LooseFileAllowed::Hook, nullptr) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize LooseFileAllowed. (Outdated index?)");
		return false;
	}

	if (MH_CreateHook(CheckFileCRCsWithServerTarget, &CheckFileCRCsWithServer::Hook, reinterpret_cast<void**>(&CheckFileCRCsWithServerOriginal)) != MH_OK)
	{
		throw std::runtime_error("Failed to initialize CheckFileCRCsWithServer. (Outdated index?)");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		throw std::runtime_error("Failed to enable hooks.");
		return false;
	}

	return true;
}

void Hooks::Release()
{
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
	SequenceHook->~RecvPropHook();
}

bool __stdcall Hooks::CreateMove::Hook(float inputSampleFrametime, CUserCmd* cmd)
{
	CreateMoveOriginal(inputSampleFrametime, cmd);

	if (!cmd || !cmd->command_number)
		return CreateMoveOriginal(inputSampleFrametime, cmd);

	g_Movement.BunnyHop(cmd);

	if (g_LocalPlayer && InputSys::Get().IsKeyDown(VK_TAB) && g_Configs.misc.rankReveal)
		Utils::RankRevealAll();

	int OldFlags = g_LocalPlayer->m_fFlags();
	EnginePrediction::Begin(cmd);
	{

		if (InputSys::Get().IsKeyDown(g_Configs.misc.edgeJumpKey))
		{
			if ((OldFlags & FL_ONGROUND) && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
				cmd->buttons |= IN_JUMP;

			if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
				cmd->buttons |= IN_DUCK;
		}

	}
	EnginePrediction::End();

	return false;
}

void __stdcall Hooks::EmitSound::Hook(IRecipientFilter& filter, int entIndex, int channel, const char* soundEntry, unsigned int soundEntryHash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const Vector* origin, const Vector* direction, void* utlVecOrigins, bool updatePositions, float soundTime, int speakerEntity, int unk)
{
	if (!strcmp(soundEntry, "UIPanorama.popup_accept_match_beep") && g_Configs.misc.autoAccept)
	{
		static auto AcceptFn = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
		HWND Hwnd;
		if ((Hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive")) && GetForegroundWindow() == Hwnd)
		{
			RECT lprect;
			GetClientRect(Hwnd, &lprect);
			SendMessage(Hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(lprect.right / 2, lprect.bottom / 2)); 
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}

		if (AcceptFn)
			AcceptFn("");
	}

	EmitSoundOriginal(g_EngineSound, filter, entIndex, channel, soundEntry, soundEntryHash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utlVecOrigins, updatePositions, soundTime, speakerEntity, unk);
}

bool __stdcall Hooks::FireEvent::Hook(IGameEvent* event) {
	if (!strcmp(event->GetName(), "player_death") && g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer())
	{
		auto& weapon = g_LocalPlayer->m_hActiveWeapon();
		if (!weapon)
			return false;

		if (weapon && weapon->IsWeapon())
		{
			auto& skin_data = g_Configs.skins.m_Items[weapon->m_Item().m_iItemDefinitionIndex()];
			if (skin_data.enabled && skin_data.stattrak)
			{
				skin_data.stattrak++;
				weapon->m_nFallbackStatTrak() = skin_data.stattrak;
				weapon->GetClientNetworkable()->PostDataUpdate(0);
				weapon->GetClientNetworkable()->OnDataChanged(0);
			}
		}

		const auto iconOverride = g_Configs.skins.GetIconOverride(event->GetString("weapon"));
		if (iconOverride)
		{
			event->SetString("weapon", iconOverride);
		}
	}

	return FireEventOriginal(g_GameEvents, event);
}

void __stdcall Hooks::FrameStageNotify::Hook(ClientFrameStage_t stage)
{
	if (g_EngineClient->IsInGame()) 
	{
		Models::PlayerChanger(stage);
		Models::KnifeChanger(stage);
		Models::AWPChanger(stage);
		Skins::Run(stage);
	}

	FrameStageNotifyOriginal(g_CHLClient, stage);
}

static int RandomSequence(const int low, const int high)
{
	return rand() % (high - low + 1) + low;
}

static int FixAnimation(const uint32_t model, const int sequence)
{
	enum ESequence
	{
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT01 = 12,

		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT01 = 12,
		SEQUENCE_FALCHION_LOOKAT02 = 13,

		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

		SEQUENCE_BOWIE_IDLE1 = 1,
	};

	switch (model) {
	case FNV::Hash("models/weapons/v_knife_butterfly.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default:
			return sequence + 1;
		}
	}
	case FNV::Hash("models/weapons/v_knife_falchion_advanced.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	case FNV::Hash("models/weapons/v_knife_push.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	case FNV::Hash("models/weapons/v_knife_survival_bowie.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}
	case FNV::Hash("models/weapons/v_knife_ursus.mdl"):
	case FNV::Hash("models/weapons/v_knife_skeleton.mdl"):
	case FNV::Hash("models/weapons/v_knife_outdoor.mdl"):
	case FNV::Hash("models/weapons/v_knife_cord.mdl"):
	case FNV::Hash("models/weapons/v_knife_canis.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	}
	case FNV::Hash("models/weapons/v_knife_stiletto.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(12, 13);
		}
	}
	case FNV::Hash("models/weapons/v_knife_widowmaker.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(14, 15);
		}
	}
	default:
		return sequence;
	}
}

void Hooks::RecvProxy::Hook(const CRecvProxyData* data, void* entity, void* output)
{
	static auto RecvProxyOriginal = SequenceHook->GetOriginal();
	if (g_LocalPlayer && g_LocalPlayer->IsAlive())
	{
		const auto proxyData = const_cast<CRecvProxyData*>(data);
		const auto viewModel = static_cast<CBaseViewModel*>(entity);
		if (viewModel && viewModel->m_hOwner() && viewModel->m_hOwner().IsValid())
		{
			const auto owner = static_cast<CBasePlayer*>(g_EntityList->GetClientEntityFromHandle(viewModel->m_hOwner()));
			if (owner == g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()))
			{
				const auto viewModelWeaponHandle = viewModel->m_hWeapon();
				if (viewModelWeaponHandle.IsValid())
				{
					const auto viewModelWeapon = static_cast<CBaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(viewModelWeaponHandle));
					if (viewModelWeapon)
					{
						if (k_WeaponInfo.count(viewModelWeapon->m_Item().m_iItemDefinitionIndex()))
						{
							auto originalSequence = proxyData->m_Value.m_Int;
							const auto overrideModel = k_WeaponInfo.at(viewModelWeapon->m_Item().m_iItemDefinitionIndex()).model;
							proxyData->m_Value.m_Int = FixAnimation(FNV::HashRuntime(overrideModel), proxyData->m_Value.m_Int);
						}
					}
				}
			}
		}
	}

	RecvProxyOriginal(data, entity, output);
}

void __stdcall Hooks::LockCursor::Hook()
{
	if (Menu.Opened)
	{
		g_VGuiSurface->UnlockCursor();
		return;
	}

	LockCursorOriginal(g_VGuiSurface);
}

static bool Initialized = false;
long __stdcall Hooks::EndScene::Hook(IDirect3DDevice9* device)
{
	static uintptr_t GameOverlayReturnAddress = 0;
	if (!GameOverlayReturnAddress) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

		char mod[MAX_PATH];
		GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

		if (strstr(mod, "gameoverlay"))
			GameOverlayReturnAddress = (uintptr_t)(_ReturnAddress());
	}

	if (GameOverlayReturnAddress != (uintptr_t)(_ReturnAddress()))
		return EndSceneOriginal(device);

	if (!Initialized)
	{
		Menu.ApplyFonts();
		Menu.SetupEndScene(device);
		Initialized = true;
	}

	if (Initialized)
	{
		Menu.PreRender(device);
		Menu.PostRender();

		Menu.Run();
		Menu.RunPopup();
		Menu.EndEndScene(device);
	}

	return EndSceneOriginal(device);
}

long __stdcall Hooks::Reset::Hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters)
{
	if (!Initialized)
		ResetOriginal(device, presentationParameters);

	Menu.InvalidateObjects();
	long hr = ResetOriginal(device, presentationParameters);
	Menu.CreateObjects(device);

	return hr;
}

bool __fastcall Hooks::LooseFileAllowed::Hook(void* ecx, void* edx)
{
	return true;
}

void __fastcall Hooks::CheckFileCRCsWithServer::Hook(void* ecx, void* edx)
{
	return;
}