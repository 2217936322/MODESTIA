#pragma once

#include <Windows.h>

#include "Misc/VFunc.hpp"

#include "Misc/Enums.hpp"

#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "Misc/Studio.hpp"

#include "Interfaces/IAppSystem.hpp"
#include "Interfaces/IBaseClientDll.hpp"
#include "Interfaces/IClientEntity.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/IClientMode.hpp"
#include "Interfaces/IClientLeafSystem.hpp"
#include "Interfaces/IConVar.hpp"
#include "Interfaces/ICvar.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IVDebugOverlay.hpp"
#include "Interfaces/ISurface.hpp"
#include "Interfaces/IVModelInfoClient.hpp"
#include "Interfaces/IVModelRender.hpp"
#include "Interfaces/IRenderView.hpp"
#include "Interfaces/IGameEventmanager.hpp"
#include "Interfaces/IMaterialSystem.hpp"
#include "Interfaces/IMoveHelper.hpp"
#include "Interfaces/IMDLCache.hpp"
#include "Interfaces/IPrediction.hpp"
#include "Interfaces/IPanel.hpp"
#include "Interfaces/IEngineSound.hpp"
#include "Interfaces/IViewRender.hpp"
#include "Interfaces/CClientState.hpp"
#include "Interfaces/IPhysics.hpp"
#include "Interfaces/IInputSystem.hpp"
#include "interfaces/IRefCounted.hpp"
#include "interfaces/IMemAlloc.hpp"
#include "Interfaces/ILocalize.hpp"
#include "Interfaces/IFileSystem.hpp"
#include "Interfaces/INetworkStringTable.hpp"

#include "Misc/Convar.hpp"
#include "Misc/CUserCmd.hpp"
#include "Misc/datamap.hpp"

#include "netvars.hpp"

struct IDirect3DDevice9;

namespace Interfaces
{
	void Initialize();
	void Dump();
}

extern IVEngineClient*       g_EngineClient;
extern IBaseClientDLL*       g_CHLClient;
extern IClientEntityList*    g_EntityList;
extern IClientLeafSystem*	 g_ClientLeafSystem;
extern CGlobalVarsBase*      g_GlobalVars;
extern IEngineTrace*         g_EngineTrace;
extern ICvar*                g_CVar;
extern IPanel*               g_VGuiPanel;
extern IClientMode*          g_ClientMode;
extern IVDebugOverlay*       g_DebugOverlay;
extern ISurface*             g_VGuiSurface;
extern IVModelInfoClient*    g_MdlInfo;
extern IVModelRender*        g_MdlRender;
extern IVRenderView*         g_RenderView;
extern IMaterialSystem*      g_MatSystem;
extern IGameEventManager2*   g_GameEvents;
extern IMoveHelper*          g_MoveHelper;
extern IMDLCache*            g_MdlCache;
extern IPrediction*          g_Prediction;
extern CGameMovement*        g_GameMovement;
extern IEngineSound*         g_EngineSound;
extern IViewRender*          g_ViewRender;
extern IDirect3DDevice9*     g_D3DDevice9;
extern CClientState*         g_ClientState;
extern IPhysicsSurfaceProps* g_PhysSurface;
extern ILocalize*			 g_Localize;
extern IInputSystemtem*         g_InputSystemtem;
extern IMemAlloc*			 g_MemAlloc;
extern uintptr_t*			 g_SpatialPartition;
extern IFileSystem*			 g_FileSystem;
extern INetworkStringTableContainer* g_NetworkStringTableContainer;

template<typename... Args>
void ConMsg(const char* pMsg, Args... args) {
	static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
	return import(pMsg, args...);
}

template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args) {
	static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
	return import(clr, pMsg, args...);
}

#include "Misc/EHandle.hpp"

class C_LocalPlayer
{
public:
	C_LocalPlayer() : m_Local(nullptr) { }

	operator bool() const { return *m_Local != nullptr; }
	operator CBasePlayer* () const { return *m_Local; }

	CBasePlayer* operator->() { return *m_Local; }

private:
	CBasePlayer** m_Local;
};

extern C_LocalPlayer g_LocalPlayer;

