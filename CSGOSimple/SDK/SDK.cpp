#include "sdk.hpp"
#include "csgostructs.hpp"

#include "../Helpers/Utils.hpp"

IVEngineClient*       g_EngineClient     = nullptr;
IBaseClientDLL*       g_CHLClient        = nullptr;
IClientEntityList*    g_EntityList       = nullptr;
IClientLeafSystem*    g_ClientLeafSystem = nullptr;
CGlobalVarsBase*      g_GlobalVars       = nullptr;
IEngineTrace*         g_EngineTrace      = nullptr;
ICvar*                g_CVar             = nullptr;
IPanel*               g_VGuiPanel        = nullptr;
IClientMode*          g_ClientMode       = nullptr;
IVDebugOverlay*       g_DebugOverlay     = nullptr;
ISurface*             g_VGuiSurface      = nullptr;
IVModelInfoClient*    g_MdlInfo          = nullptr;
IVModelRender*        g_MdlRender        = nullptr;
IVRenderView*         g_RenderView       = nullptr;
IMaterialSystem*      g_MatSystem        = nullptr;
IGameEventManager2*   g_GameEvents       = nullptr;
IMoveHelper*          g_MoveHelper       = nullptr;
IMDLCache*            g_MdlCache         = nullptr;
IPrediction*          g_Prediction       = nullptr;
CGameMovement*        g_GameMovement     = nullptr;
IEngineSound*         g_EngineSound      = nullptr;
IViewRender*          g_ViewRender       = nullptr;
IDirect3DDevice9*     g_D3DDevice9       = nullptr;
CClientState*         g_ClientState      = nullptr;
IPhysicsSurfaceProps* g_PhysSurface      = nullptr;
IInputSystemtem*         g_InputSystemtem      = nullptr;
IMemAlloc*			  g_MemAlloc		 = nullptr;
uintptr_t*			  g_SpatialPartition = nullptr;
C_LocalPlayer         g_LocalPlayer;
ILocalize*			  g_Localize		 = nullptr;
IFileSystem*          g_FileSystem       = nullptr;
INetworkStringTableContainer* g_NetworkStringTableContainer = nullptr;

namespace Interfaces
{
    CreateInterfaceFn GetModuleFactory(HMODULE module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }

    template<typename T>
    T* GetInterface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if(!result) {
            throw std::runtime_error(std::string("[GetInterface] Failed to GetOffset interface: ") + szInterfaceVersion);
        }

        return result;
    }

    void Initialize()
    {
	    auto client            = GetModuleHandle("client.dll");
		auto engine            = GetModuleHandle("engine.dll");
		auto dx9api            = GetModuleHandle("shaderapidx9.dll");
		auto stdlib            = GetModuleHandle("vstdlib.dll");
        auto filesystemFactory = GetModuleFactory(GetModuleHandle("filesystem_stdio.dll"));
        auto engineFactory     = GetModuleFactory(GetModuleHandle("engine.dll"));
        auto clientFactory     = GetModuleFactory(GetModuleHandle("client.dll"));
        auto vguiFactory       = GetModuleFactory(GetModuleHandle("vguimatsurface.dll"));
        auto vgui2Factory      = GetModuleFactory(GetModuleHandle("vgui2.dll"));
        auto matSysFactory     = GetModuleFactory(GetModuleHandle("materialsystem.dll"));
        auto dataCacheFactory  = GetModuleFactory(GetModuleHandle("datacache.dll"));
        auto vphysicsFactory   = GetModuleFactory(GetModuleHandle("vphysics.dll"));
        auto InputSystemFactory   = GetModuleFactory(GetModuleHandle("InputSystemtem.dll"));
		auto localizeFactory   = GetModuleFactory(GetModuleHandle("localize.dll"));
		auto studioFactory     = GetModuleFactory(GetModuleHandle("studiorender.dll"));
        auto valveStdFactory   = GetModuleFactory(stdlib);

        g_CHLClient = GetInterface<IBaseClientDLL>(clientFactory, "VClient018");
        g_EntityList = GetInterface<IClientEntityList>(clientFactory, "VClientEntityList003");
        g_ClientLeafSystem = GetInterface<IClientLeafSystem>(clientFactory, "ClientLeafSystem002");
        g_Prediction = GetInterface<IPrediction>(clientFactory, "VClientPrediction001");
        g_GameMovement = GetInterface<CGameMovement>(clientFactory, "GameMovement001");
        g_MdlCache = GetInterface<IMDLCache>(dataCacheFactory, "MDLCache004");
        g_EngineClient = GetInterface<IVEngineClient>(engineFactory, "VEngineClient014");
        g_MdlInfo = GetInterface<IVModelInfoClient>(engineFactory, "VModelInfoClient004");
        g_MdlRender = GetInterface<IVModelRender>(engineFactory, "VEngineModel016");
        g_RenderView = GetInterface<IVRenderView>(engineFactory, "VEngineRenderView014");
        g_EngineTrace = GetInterface<IEngineTrace>(engineFactory, "EngineTraceClient004");
        g_DebugOverlay = GetInterface<IVDebugOverlay>(engineFactory, "VDebugOverlay004");
        g_GameEvents = GetInterface<IGameEventManager2>(engineFactory, "GAMEEVENTSMANAGER002");
        g_EngineSound = GetInterface<IEngineSound>(engineFactory, "IEngineSoundClient003");
        g_NetworkStringTableContainer = GetInterface<INetworkStringTableContainer>(engineFactory, "VEngineClientStringTable001");
        g_MatSystem = GetInterface<IMaterialSystem>(matSysFactory, "VMaterialSystem080");
        g_CVar = GetInterface<ICvar>(valveStdFactory, "VEngineCvar007");
        g_VGuiPanel = GetInterface<IPanel>(vgui2Factory, "VGUI_Panel009");
        g_VGuiSurface = GetInterface<ISurface>(vguiFactory, "VGUI_Surface031");
        g_PhysSurface = GetInterface<IPhysicsSurfaceProps>(vphysicsFactory, "VPhysicsSurfaceProps001");
        g_InputSystemtem = GetInterface<IInputSystemtem>(InputSystemFactory, "InputSystemtemVersion001");
		g_SpatialPartition = GetInterface<uintptr_t>(engineFactory, "SpatialPartition001" );
		g_Localize = GetInterface<ILocalize>(localizeFactory, "Localize_001");
        g_FileSystem = GetInterface<IFileSystem>(filesystemFactory, "VFileSystem017");
		g_MemAlloc = *(IMemAlloc**)GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc");
        g_GlobalVars = **(CGlobalVarsBase***)(Utils::PatternScan(client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
		g_ClientMode = *(IClientMode**)(Utils::PatternScan(client, "B9 ? ? ? ? E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 53") + 1);
        g_MoveHelper = **(IMoveHelper***)(Utils::PatternScan(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
        g_ViewRender = *(IViewRender**)(Utils::PatternScan(client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
        g_D3DDevice9 = **(IDirect3DDevice9***)(Utils::PatternScan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
        g_ClientState = **(CClientState***)(Utils::PatternScan(engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		g_GameRules = *(CGameRules**)( Utils::PatternScan(client, "8B 0D ?? ?? ?? ?? 85 C0 74 0A 8B 01 FF 50 78 83 C0 54") + 2);
        g_LocalPlayer = *(C_LocalPlayer*)(Utils::PatternScan(client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
    }

    void Dump()
    {
        #define STRINGIFY_IMPL(s) #s
        #define STRINGIFY(s)      STRINGIFY_IMPL(s)
        #define PRINT_INTERFACE(name) Utils::ConsolePrint("%-20s: %p\n", STRINGIFY(name), name)

        PRINT_INTERFACE(g_CHLClient);
        PRINT_INTERFACE(g_EntityList);
        PRINT_INTERFACE(g_Prediction);
        PRINT_INTERFACE(g_GameMovement);
        PRINT_INTERFACE(g_MdlCache);
        PRINT_INTERFACE(g_EngineClient);
        PRINT_INTERFACE(g_MdlInfo);
        PRINT_INTERFACE(g_MdlRender);
        PRINT_INTERFACE(g_RenderView);
        PRINT_INTERFACE(g_EngineTrace);
        PRINT_INTERFACE(g_DebugOverlay);
        PRINT_INTERFACE(g_GameEvents);
        PRINT_INTERFACE(g_EngineSound);
        PRINT_INTERFACE(g_MatSystem);
        PRINT_INTERFACE(g_CVar);
        PRINT_INTERFACE(g_VGuiPanel);
        PRINT_INTERFACE(g_VGuiSurface);
        PRINT_INTERFACE(g_PhysSurface);
        PRINT_INTERFACE(g_InputSystemtem);
        PRINT_INTERFACE(g_ClientLeafSystem);
        PRINT_INTERFACE(g_FileSystem);
        PRINT_INTERFACE(g_NetworkStringTableContainer);
    }
}
