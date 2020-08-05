#pragma once

#include <d3d9.h>

#include "SDK/SDK.hpp"

namespace Hooks
{
	bool Initialize();
	void Release();

	namespace CreateMove
	{
		using Fn = bool(__stdcall*)(float, CUserCmd*);
		bool __stdcall Hook(float InputSampleFrametime, CUserCmd* Cmd);
	};

	namespace EmitSound
	{
		using Fn = void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);
		void __stdcall Hook(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	};

	namespace FireEvent
	{
		using Fn = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
		bool __stdcall Hook(IGameEvent* pEvent);
	};

	namespace FrameStageNotify
	{
		using Fn = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
		void __stdcall Hook(ClientFrameStage_t stage);
	};

	namespace RecvProxy
	{
		void Hook(const CRecvProxyData* pData, void* entity, void* output);
	};

	namespace LockCursor
	{
		using Fn = void(__thiscall*)(ISurface*);
		void __stdcall Hook();
	};

	namespace EndScene
	{
		using Fn = long(__stdcall*)(IDirect3DDevice9*);
		long __stdcall Hook(IDirect3DDevice9* device);
	};

	namespace Reset
	{
		using Fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall Hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	};

	namespace PaintTraverse
	{
		using Fn = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
		void __stdcall Hook(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	};

	namespace LooseFileAllowed
	{
		bool __fastcall Hook(void*, void*);
	};

	namespace CheckFileCRCsWithServer
	{
		using Fn = void(__thiscall*)(void*, void*);
		void __fastcall Hook(void*, void*);
	};
}