#include "Hooks.hpp"

#include <intrin.h>

#include "Helpers/Fnv.hpp"
#include "Helpers/InputSystem.hpp"
#include "Helpers/FixAnimations.hpp"
#include "Helpers/MinHook/minhook.h"
#include "Helpers/Configs/Configs.hpp"

#include "Features/Misc.hpp"
#include "Features/Movement.hpp"
#include "Features/SkinChanger.hpp"
#include "Features/ModelChanger.hpp"
#include "Features/EnginePrediction.hpp"
#include "Helpers/ItemDefinitions.hpp"
#include "Menu/Menu.hpp"

RecvPropHook* SequenceHook;
Hooks::CreateMove::Fn CreateMoveOriginal = nullptr;
Hooks::EmitSound::Fn EmitSoundOriginal = nullptr;
Hooks::FireEvent::Fn FireEventOriginal = nullptr;
Hooks::FrameStageNotify::Fn FrameStageNotifyOriginal = nullptr;
Hooks::LockCursor::Fn LockCursorOriginal = nullptr;
Hooks::EndScene::Fn EndSceneOriginal = nullptr;
Hooks::Reset::Fn ResetOriginal = nullptr;
Hooks::SendNetMessage::Fn SendNetMessageOriginal = nullptr;

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

	if (MH_Initialize() != MH_OK) { throw std::runtime_error("Failed to initialize MH_Initialize."); return false; }

	if (MH_CreateHook(CreateMoveTarget, &CreateMove::Hook, reinterpret_cast<void**>(&CreateMoveOriginal)) != MH_OK) { throw std::runtime_error("Failed to create CreateMoveHook. (Wrong index?)"); return false; }
	if (MH_CreateHook(EmitSoundTarget, &EmitSound::Hook, reinterpret_cast<void**>(&EmitSoundOriginal)) != MH_OK) { throw std::runtime_error("Failed to create EmitSoundHook. (Wrong index?)"); return false; }
	if (MH_CreateHook(FireEventTarget, &FireEvent::Hook, reinterpret_cast<void**>(&FireEventOriginal)) != MH_OK) { throw std::runtime_error("Failed to create FireEventHook. (Wrong index?)"); return false; }
	if (MH_CreateHook(FrameStageNotifyTarget, &FrameStageNotify::Hook, reinterpret_cast<void**>(&FrameStageNotifyOriginal)) != MH_OK) { throw std::runtime_error("Failed to create FrameStageNotifyHook. (Wrong index?)"); return false; }
	if (MH_CreateHook(LockCursorTarget, &LockCursor::Hook, reinterpret_cast<void**>(&LockCursorOriginal)) != MH_OK) { throw std::runtime_error("Failed to create LockCursorHook. (Wrong index?)"); return false; }
	if (MH_CreateHook(EndSceneTarget, &EndScene::Hook, reinterpret_cast<void**>(&EndSceneOriginal)) != MH_OK) { throw std::runtime_error("Failed to create EndSceneHook. (Wrong index?)"); return false; }
	if (MH_CreateHook(ResetTarget, &Reset::Hook, reinterpret_cast<void**>(&ResetOriginal)) != MH_OK) { throw std::runtime_error("Failed to create ResetHook. (Wrong index?)");return false; }
	SequenceHook = new RecvPropHook(CBaseViewModel::m_nSequence(), Hooks::RecvProxy::Hook);

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) { throw std::runtime_error("Failed to enable hooks."); return false; }

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

	if (!cmd || !cmd->m_iCommandNumber)
		return CreateMoveOriginal(inputSampleFrametime, cmd);

	Misc::Get().RankReveal(cmd);
	Misc::Get().ClantagChanger();

	static void* netChannelOld = nullptr;
	auto netChannel = g_ClientState->m_NetChannel;

	if (netChannelOld != netChannel && netChannel)
	{
		netChannelOld = netChannel;

		auto SendNetMessageTarget = reinterpret_cast<void*>(GetVirtual(netChannel, 40));

		if (MH_CreateHook(SendNetMessageTarget, &SendNetMessage::Hook, reinterpret_cast<void**>(&SendNetMessageOriginal)) != MH_OK) { throw std::runtime_error("Failed to create SendNetMessageHook. (Wrong index?)");return false; }

		if (MH_EnableHook(SendNetMessageTarget) != MH_OK) { throw std::runtime_error("Failed to enable SendNetMessageHook."); return false; }
	}

	EnginePrediction::Run(cmd);

	Movement::Get().EdgeJump(cmd);

	return false;
}

void __stdcall Hooks::EmitSound::Hook(IRecipientFilter& filter, int entIndex, int channel, const char* soundEntry, unsigned int soundEntryHash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const Vector* origin, const Vector* direction, void* utlVecOrigins, bool updatePositions, float soundTime, int speakerEntity, int unk)
{
	if (!strcmp(soundEntry, "UIPanorama.popup_accept_match_beep") && g_Configs.misc.autoAccept)
	{
		static auto acceptFn = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandle("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
		HWND hwnd;
		if ((hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive")) && GetForegroundWindow() == hwnd)
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			SendMessage(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(rect.right / 2, rect.bottom / 2));
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}

		if (acceptFn)
			acceptFn("");
	}

	EmitSoundOriginal(g_EngineSound, filter, entIndex, channel, soundEntry, soundEntryHash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utlVecOrigins, updatePositions, soundTime, speakerEntity, unk);
}

bool __stdcall Hooks::FireEvent::Hook(IGameEvent* event) 
{
	if (!strcmp(event->GetName(), "player_death") && g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer())
	{
		auto& weapon = g_LocalPlayer->m_hActiveWeapon();
		if (!weapon)
			return false;

		auto& skinData = g_Configs.skinChanger.m_Items[weapon->m_Item().m_iItemDefinitionIndex()];
		if (skinData.enabled && skinData.stattrak)
		{
			skinData.stattrak++;
			weapon->m_nFallbackStatTrak() = skinData.stattrak;
			weapon->GetClientNetworkable()->PostDataUpdate(0);
			weapon->GetClientNetworkable()->OnDataChanged(0);
		}

		const auto iconOverride = g_Configs.skinChanger.GetIconOverride(event->GetString("weapon"));
		if (iconOverride)
			event->SetString("weapon", iconOverride);
	}

	return FireEventOriginal(g_GameEvents, event);
}

void __stdcall Hooks::FrameStageNotify::Hook(ClientFrameStage_t stage)
{
	if (g_EngineClient->IsInGame()) 
	{
		ModelChanger::Get().PlayerChanger(stage);
		SkinChanger::Get().Run(stage);
	}

	FrameStageNotifyOriginal(g_CHLClient, stage);
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
						if (WeaponInformations.count(viewModelWeapon->m_Item().m_iItemDefinitionIndex()))
						{
							auto originalSequence = proxyData->m_Value.m_Int;
							const auto overrideModel = WeaponInformations.at(viewModelWeapon->m_Item().m_iItemDefinitionIndex()).model;
							proxyData->m_Value.m_Int = FixAnimations(FNV::HashRuntime(overrideModel), proxyData->m_Value.m_Int);
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
	if (!GameOverlayReturnAddress)
	{
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

bool __fastcall Hooks::SendNetMessage::Hook(void* networkChannel, void* edx, INetworkMessage& message, bool forceReliable, bool voice)
{
	if (message.GetType() == 14) // sv_pure bypass
		return false;

	if (message.GetGroup() == 9) // fix lag when transmitting voice
		voice = true;

	return SendNetMessageOriginal(networkChannel, message, forceReliable, voice);
}