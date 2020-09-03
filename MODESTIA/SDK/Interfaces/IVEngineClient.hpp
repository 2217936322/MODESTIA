#pragma once

#include "../Math/Vector.hpp"
#include "../Math/Vector2D.hpp"
#include "../Math/QAngle.hpp"
#include "../Math/VMatrix.hpp"

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		

#define SIGNONSTATE_NONE		0	
#define SIGNONSTATE_CHALLENGE	1	
#define SIGNONSTATE_CONNECTED	2	
#define SIGNONSTATE_NEW			3	
#define SIGNONSTATE_PRESPAWN	4	
#define SIGNONSTATE_SPAWN		5
#define SIGNONSTATE_FULL		6	
#define SIGNONSTATE_CHANGELEVEL	7	

typedef struct InputContextHandle_t__* InputContextHandle_t;
struct ClientTextMessage_t;
struct Model_t;
class SurfInfo;
class IMaterial;
class CSentence;
class CAudioSource;
class AudioState_t;
class ISpatialQuery;
class IMaterialSystem;
class CPhysCollide;
class IAchievementMgr;
class ISPSharedMemory;
class CGamestatsData;
class KeyValues;
class CSteamAPIContext;
struct Frustum_t;

typedef void(*pfnDemoCustomDataCallback)(uint8_t* pData, size_t iSize);

class INetChannelInfo
{
public:

	enum 
	{
		GENERIC = 0,	
		LOCALPLAYER,	
		OTHERPLAYERS,	
		ENTITIES,		
		SOUNDS,			
		EVENTS,			
		USERMESSAGES,	
		ENTMESSAGES,	
		VOICE,			
		STRINGTABLE,	
		MOVE,			
		STRINGCMD,		
		SIGNON,			
		TOTAL,		
	};

	virtual const char* GetName(void) const = 0;	
	virtual const char* GetAddress(void) const = 0; 
	virtual float		GetTime(void) const = 0;	
	virtual float		GetTimeConnected(void) const = 0;	
	virtual int			GetBufferSize(void) const = 0;
	virtual int			GetDataRate(void) const = 0; 

	virtual bool		IsLoopback(void) const = 0;	
	virtual bool		IsTimingOut(void) const = 0;	
	virtual bool		IsPlayback(void) const = 0;	

	virtual float		GetLatency(int flow) const = 0;	 
	virtual float		GetAvgLatency(int flow) const = 0; 
	virtual float		GetAvgLoss(int flow) const = 0;	 
	virtual float		GetAvgChoke(int flow) const = 0;	 
	virtual float		GetAvgData(int flow) const = 0;	
	virtual float		GetAvgPackets(int flow) const = 0;
	virtual int			GetTotalData(int flow) const = 0;	 
	virtual int			GetSequenceNr(int flow) const = 0;	
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0;
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; 
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; 
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  
	virtual float		GetTimeSinceLastReceived(void) const = 0;	
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;

	std::byte pad[24];
	int m_iOutSequenceNr;
	int m_iInSequenceNr;
	int m_iOutSequenceNrAck;
	int m_iOutReliableState;
	int m_iInReliableState;
	int m_iChokedPackets;
};

class INetworkMessage
{
public:
	int GetGroup()
	{
		using Fn = int(__thiscall*)(INetworkMessage*);
		return CallVirtualFunction<Fn>(this, 6)(this);
	}

	int GetType()
	{
		using Fn = int(__thiscall*)(INetworkMessage*);
		return CallVirtualFunction<Fn>(this, 7)(this);
	}
};

struct PlayerInfo_t
{
	int64_t __pad0;
	union 
	{
		int64_t m_iXUID;
		struct 
		{
			int m_iXUIDLow;
			int m_iXUIDHigh;
		};
	};

	char m_szName[128];
	int m_iUserID;
	char szGUID[33];
	unsigned int m_iFriendsID;
	char m_szFriendsName[128];
	bool m_bFakeplayer;
	bool m_bIsHLTV;
	unsigned int m_iCustomFiles[4];
	unsigned char m_szFilesDownloaded;
};

class IMapOverview 
{
public:
	virtual	~IMapOverview(void) {};

	virtual	void SetVisible(bool state) = 0;	
	virtual void SetBounds(int x, int y, int wide, int tall) = 0; 
	virtual void SetZoom(float zoom) = 0;
	virtual void SetTime(float time) = 0; 
	virtual void SetAngle(float angle) = 0;
	virtual void SetFollowAngle(bool state) = 0; 
	virtual void SetCenter(Vector2D& mappos) = 0; 
	virtual void SetPlayerPositions(int index, const Vector& position, const QAngle& angle) = 0;
	virtual Vector2D WorldToMap(Vector& worldpos) = 0; 

	virtual bool  IsVisible(void) = 0;
	virtual void  GetBounds(int& x, int& y, int& wide, int& tall) = 0; 
	virtual float GetZoom(void) = 0;

	virtual	void ShowPlayerNames(bool state) = 0;	
	virtual	void ShowTracers(bool state) = 0;	
	virtual	void ShowExplosions(bool state) = 0;	
	virtual	void ShowHealth(bool state) = 0;		
	virtual	void ShowHurts(bool state) = 0;	
	virtual	void ShowTracks(float seconds) = 0;
};


class IVEngineClient
{
public:
	virtual int                   GetIntersectingSurfaces(const Model_t* model, const Vector& vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo* pInfos, const int nMaxInfos) = 0;
	virtual Vector                GetLightForPoint(const Vector& pos, bool bClamp) = 0;
	virtual IMaterial* TraceLineMaterialAndLighting(const Vector& start, const Vector& end, Vector& diffuseLightColor, Vector& baseColor) = 0;
	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool                  CopyFile(const char* source, const char* destination) = 0;
	virtual void                  GetScreenSize(int& width, int& height) = 0;
	virtual void                  ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
	virtual void                  ClientCmd(const char* szCmdString) = 0;
	virtual bool                  GetPlayerInfo(int ent_num, PlayerInfo_t* pinfo) = 0;
	virtual int                   GetPlayerForUserID(int userID) = 0;
	virtual ClientTextMessage_t* TextMessageGet(const char* pName) = 0; 
	virtual bool                  Con_IsVisible(void) = 0;
	virtual int                   GetLocalPlayer(void) = 0;
	virtual const Model_t* LoadModel(const char* pName, bool bProp = false) = 0;
	virtual float                 GetLastTimeStamp(void) = 0;
	virtual CSentence* GetSentence(CAudioSource* pAudioSource) = 0;  
	virtual float                 GetSentenceLength(CAudioSource* pAudioSource) = 0;
	virtual bool                  IsStreaming(CAudioSource* pAudioSource) const = 0;
	virtual void                  GetViewAngles(QAngle& va) = 0;
	virtual void                  SetViewAngles(QAngle& va) = 0;
	virtual int                   GetMaxClients(void) = 0;  
	virtual const char* Key_LookupBinding(const char* pBinding) = 0;
	virtual const char* Key_BindingForKey(int& code) = 0;
	virtual void                  Key_SetBinding(int, char const*) = 0;
	virtual void                  StartKeyTrapMode(void) = 0;
	virtual bool                  CheckDoneKeyTrapping(int& code) = 0;
	virtual bool                  IsInGame(void) = 0;
	virtual bool                  IsConnected(void) = 0;
	virtual bool                  IsDrawingLoadingImage(void) = 0;
	virtual void                  HideLoadingPlaque(void) = 0;
	virtual void                  Con_NPrintf(int pos, const char* fmt, ...) = 0;  
	virtual void                  Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
	virtual int                   IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;
	virtual int                   IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;
	virtual bool                  CullBox(const Vector& mins, const Vector& maxs) = 0;
	virtual void                  Sound_ExtraUpdate(void) = 0;
	virtual const char* GetGameDirectory(void) = 0;
	virtual const VMatrix& WorldToScreenMatrix() = 0;
	virtual const VMatrix& WorldToViewMatrix() = 0;
	virtual int                   GameLumpVersion(int lumpId) const = 0;
	virtual int                   GameLumpSize(int lumpId) const = 0;  
	virtual bool                  LoadGameLump(int lumpId, void* pBuffer, int size) = 0;
	virtual int                   LevelLeafCount() const = 0;
	virtual ISpatialQuery* GetBSPTreeQuery() = 0;
	virtual void                  LinearToGamma(float* linear, float* gamma) = 0;
	virtual float                 LightStyleValue(int style) = 0;  
	virtual void                  ComputeDynamicLighting(const Vector& pt, const Vector* pNormal, Vector& color) = 0;
	virtual void                  GetAmbientLightColor(Vector& color) = 0;
	virtual int                   GetDXSupportLevel() = 0;
	virtual bool                  SupportsHDR() = 0;
	virtual void                  Mat_Stub(IMaterialSystem* pMatSys) = 0;  
	virtual void                  GetChapterName(char* pchBuff, int iMaxLength) = 0;
	virtual char const* GetLevelName(void) = 0;
	virtual char const* GetLevelNameShort(void) = 0;
	virtual char const* GetMapGroupName(void) = 0;
	virtual struct IVoiceTweak_s* GetVoiceTweakAPI(void) = 0;
	virtual void                  SetVoiceCasterID(unsigned int someint) = 0;  
	virtual void                  EngineStats_BeginFrame(void) = 0;
	virtual void                  EngineStats_EndFrame(void) = 0;
	virtual void                  FireEvents() = 0;
	virtual int                   GetLeavesArea(unsigned short* pLeaves, int nLeaves) = 0;
	virtual bool                  DoesBoxTouchAreaFrustum(const Vector& mins, const Vector& maxs, int iArea) = 0;  
	virtual int                   GetFrustumList(Frustum_t** pList, int listMax) = 0;
	virtual bool                  ShouldUseAreaFrustum(int i) = 0;
	virtual void                  SetAudioState(const AudioState_t& state) = 0;
	virtual int                   SentenceGroupPick(int groupIndex, char* name, int nameBufLen) = 0;
	virtual int                   SentenceGroupPickSequential(int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset) = 0;
	virtual int                   SentenceIndexFromName(const char* pSentenceName) = 0;
	virtual const char* SentenceNameFromIndex(int sentenceIndex) = 0;
	virtual int                   SentenceGroupIndexFromName(const char* pGroupName) = 0;
	virtual const char* SentenceGroupNameFromIndex(int groupIndex) = 0;
	virtual float                 SentenceLength(int sentenceIndex) = 0;
	virtual void                  ComputeLighting(const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector* pBoxColors = NULL) = 0;
	virtual void                  ActivateOccluder(int nOccluderIndex, bool bActive) = 0;
	virtual bool                  IsOccluded(const Vector& vecAbsMins, const Vector& vecAbsMaxs) = 0;  
	virtual int                   GetOcclusionViewId(void) = 0;
	virtual void* SaveAllocMemory(size_t num, size_t size) = 0;
	virtual void                  SaveFreeMemory(void* pSaveMem) = 0;
	virtual INetChannelInfo* GetNetChannelInfo(void) = 0;
	virtual void                  DebugDrawPhysCollide(const CPhysCollide* pCollide, IMaterial* pMaterial, const Matrix3x4_t& transform, const uint8_t* color) = 0; //79
	virtual void                  CheckPoint(const char* pName) = 0;  
	virtual void                  DrawPortals() = 0;
	virtual bool                  IsPlayingDemo(void) = 0;
	virtual bool                  IsRecordingDemo(void) = 0;
	virtual bool                  IsPlayingTimeDemo(void) = 0;
	virtual int                   GetDemoRecordingTick(void) = 0;
	virtual int                   GetDemoPlaybackTick(void) = 0;
	virtual int                   GetDemoPlaybackStartTick(void) = 0;
	virtual float                 GetDemoPlaybackTimeScale(void) = 0;
	virtual int                   GetDemoPlaybackTotalTicks(void) = 0;
	virtual bool                  IsPaused(void) = 0;  
	virtual float                 GetTimescale(void) const = 0;
	virtual bool                  IsTakingScreenshot(void) = 0;
	virtual bool                  IsHLTV(void) = 0;
	virtual bool                  IsLevelMainMenuBackground(void) = 0;
	virtual void                  GetMainMenuBackgroundName(char* dest, int destlen) = 0;
	virtual void                  SetOcclusionParameters(const int& params) = 0;  
	virtual void                  GetUILanguage(char* dest, int destlen) = 0;
	virtual int                   IsSkyboxVisibleFromPoint(const Vector& vecPoint) = 0;
	virtual const char* GetMapEntitiesString() = 0;
	virtual bool                  IsInEditMode(void) = 0;  
	virtual float                 GetScreenAspectRatio(int viewportWidth, int viewportHeight) = 0;
	virtual bool                  REMOVED_SteamRefreshLogin(const char* password, bool isSecure) = 0;
	virtual bool                  REMOVED_SteamProcessCall(bool& finished) = 0;
	virtual unsigned int          GetEngineBuildNumber() = 0;   
	virtual const char* GetProductVersionString() = 0;     
	virtual void                  GrabPreColorCorrectedFrame(int x, int y, int width, int height) = 0;
	virtual bool                  IsHammerRunning() const = 0;
	virtual void                  ExecuteClientCmd(const char* szCmdString) = 0; 
	virtual bool                  MapHasHDRLighting(void) = 0;
	virtual bool                  MapHasLightMapAlphaData(void) = 0;
	virtual int                   GetAppID() = 0;
	virtual Vector                GetLightForPointFast(const Vector& pos, bool bClamp) = 0;
	virtual void                  ClientCmd_Unrestricted(char  const*, int, bool) = 0;
	virtual void                  ClientCmd_Unrestricted(const char* szCmdString) = 0;  
	virtual void                  SetRestrictServerCommands(bool bRestrict) = 0;
	virtual void                  SetRestrictClientCommands(bool bRestrict) = 0;
	virtual void                  SetOverlayBindProxy(int iOverlayID, void* pBindProxy) = 0;
	virtual bool                  CopyFrameBufferToMaterial(const char* pMaterialName) = 0;
	virtual void                  ReadConfiguration(const int iController, const bool readDefault) = 0;
	virtual void                  SetAchievementMgr(IAchievementMgr* pAchievementMgr) = 0;
	virtual IAchievementMgr* GetAchievementMgr() = 0;
	virtual bool                  MapLoadFailed(void) = 0;
	virtual void                  SetMapLoadFailed(bool bState) = 0;
	virtual bool                  IsLowViolence() = 0;
	virtual const char* GetMostRecentSaveGame(void) = 0;
	virtual void                  SetMostRecentSaveGame(const char* lpszFilename) = 0;
	virtual void                  StartXboxExitingProcess() = 0;
	virtual bool                  IsSaveInProgress() = 0;
	virtual bool                  IsAutoSaveDangerousInProgress(void) = 0;
	virtual unsigned int          OnStorageDeviceAttached(int iController) = 0;
	virtual void                  OnStorageDeviceDetached(int iController) = 0;
	virtual const char* GetSaveDirName(void) = 0;
	virtual void                  WriteScreenshot(const char* pFilename) = 0;
	virtual void                  ResetDemoInterpolation(void) = 0;
	virtual int                   GetActiveSplitScreenPlayerSlot() = 0;
	virtual int                   SetActiveSplitScreenPlayerSlot(int slot) = 0;
	virtual bool                  SetLocalPlayerIsResolvable(char const* pchContext, int nLine, bool bResolvable) = 0;
	virtual bool                  IsLocalPlayerResolvable() = 0;
	virtual int                   GetSplitScreenPlayer(int nSlot) = 0;
	virtual bool                  IsSplitScreenActive() = 0;
	virtual bool                  IsValidSplitScreenSlot(int nSlot) = 0;
	virtual int                   FirstValidSplitScreenSlot() = 0;  
	virtual int                   NextValidSplitScreenSlot(int nPreviousSlot) = 0;    
	virtual ISPSharedMemory* GetSinglePlayerSharedMemorySpace(const char* szName, int ent_num = (1 << 11)) = 0;
	virtual void                  ComputeLightingCube(const Vector& pt, bool bClamp, Vector* pBoxColors) = 0;
	virtual void                  RegisterDemoCustomDataCallback(const char* szCallbackSaveID, pfnDemoCustomDataCallback pCallback) = 0;
	virtual void                  RecordDemoCustomData(pfnDemoCustomDataCallback pCallback, const void* pData, size_t iDataLength) = 0;
	virtual void                  SetPitchScale(float flPitchScale) = 0;
	virtual float                 GetPitchScale(void) = 0;
	virtual bool                  LoadFilmmaker() = 0;
	virtual void                  UnloadFilmmaker() = 0;
	virtual void                  SetLeafFlag(int nLeafIndex, int nFlagBits) = 0;
	virtual void                  RecalculateBSPLeafFlags(void) = 0;
	virtual bool                  DSPGetCurrentDASRoomNew(void) = 0;
	virtual bool                  DSPGetCurrentDASRoomChanged(void) = 0;
	virtual bool                  DSPGetCurrentDASRoomSkyAbove(void) = 0;
	virtual float                 DSPGetCurrentDASRoomSkyPercent(void) = 0;
	virtual void                  SetMixGroupOfCurrentMixer(const char* szgroupname, const char* szparam, float val, int setMixerType) = 0;
	virtual int                   GetMixLayerIndex(const char* szmixlayername) = 0;
	virtual void                  SetMixLayerLevel(int index, float level) = 0;
	virtual int                   GetMixGroupIndex(char  const* groupname) = 0;
	virtual void                  SetMixLayerTriggerFactor(int i1, int i2, float fl) = 0;
	virtual void                  SetMixLayerTriggerFactor(char  const* char1, char  const* char2, float fl) = 0;
	virtual bool                  IsCreatingReslist() = 0;
	virtual bool                  IsCreatingXboxReslist() = 0;
	virtual void                  SetTimescale(float flTimescale) = 0;
	virtual void                  SetGamestatsData(CGamestatsData* pGamestatsData) = 0;
	virtual CGamestatsData* GetGamestatsData() = 0;
	virtual void                  GetMouseDelta(int& dx, int& dy, bool b) = 0; 
	virtual   const char* Key_LookupBindingEx(const char* pBinding, int iUserId = -1, int iStartCount = 0, int iAllowJoystick = -1) = 0;
	virtual int                   Key_CodeForBinding(char  const*, int, int, int) = 0;
	virtual void                  UpdateDAndELights(void) = 0;
	virtual int                   GetBugSubmissionCount() const = 0;
	virtual void                  ClearBugSubmissionCount() = 0;
	virtual bool                  DoesLevelContainWater() const = 0;
	virtual float                 GetServerSimulationFrameTime() const = 0;
	virtual void                  SolidMoved(class IClientEntity* pSolidEnt, class ICollideable* pSolidCollide, const Vector* pPrevAbsOrigin, bool accurateBboxTriggerChecks) = 0;
	virtual void                  TriggerMoved(class IClientEntity* pTriggerEnt, bool accurateBboxTriggerChecks) = 0;
	virtual void                  ComputeLeavesConnected(const Vector& vecOrigin, int nCount, const int* pLeafIndices, bool* pIsConnected) = 0;
	virtual bool                  IsInCommentaryMode(void) = 0;
	virtual void                  SetBlurFade(float amount) = 0;
	virtual bool                  IsTransitioningToLoad() = 0;
	virtual void                  SearchPathsChangedAfterInstall() = 0;
	virtual void                  ConfigureSystemLevel(int nCPULevel, int nGPULevel) = 0;
	virtual void                  SetConnectionPassword(char const* pchCurrentPW) = 0;
	virtual CSteamAPIContext* GetSteamAPIContext() = 0;
	virtual void                  SubmitStatRecord(char const* szMapName, unsigned int uiBlobVersion, unsigned int uiBlobSize, const void* pvBlob) = 0;
	virtual void                  ServerCmdKeyValues(KeyValues* pKeyValues) = 0; 
	virtual void                  SpherePaintSurface(const Model_t* model, const Vector& location, unsigned char chr, float fl1, float fl2) = 0;
	virtual bool                  HasPaintmap(void) = 0;
	virtual void                  EnablePaintmapRender() = 0;
	virtual void                  SphereTracePaintSurface(const Model_t* model, const Vector& position, const Vector& vec2, float radius, int& utilVecShit) = 0;
	virtual void                  RemoveAllPaint() = 0;
	virtual void                  PaintAllSurfaces(unsigned char uchr) = 0;
	virtual void                  RemovePaint(const Model_t* model) = 0;
	virtual bool                  IsActiveApp() = 0;
	virtual bool                  IsClientLocalToActiveServer() = 0;
	virtual void                  TickProgressBar() = 0;
	virtual InputContextHandle_t  GetInputContext(int id) = 0;
	virtual void                  GetStartupImage(char* filename, int size) = 0;
	virtual bool                  IsUsingLocalNetworkBackdoor(void) = 0;
	virtual void                  SaveGame(const char*, bool, char*, int, char*, int) = 0;
	virtual void                  GetGenericMemoryStats(void**) = 0;
	virtual bool                  GameHasShutdownAndFlushedMemory(void) = 0;
	virtual int                   GetLastAcknowledgedCommand(void) = 0;
	virtual void                  FinishContainerWrites(int i) = 0;
	virtual void                  FinishAsyncSave(void) = 0;
	virtual int                   GetServerTick(void) = 0;
	virtual const char* GetModDirectory(void) = 0;
	virtual bool                  AudioLanguageChanged(void) = 0;
	virtual bool                  IsAutoSaveInProgress(void) = 0;
	virtual void                  StartLoadingScreenForCommand(const char* command) = 0;
	virtual void                  StartLoadingScreenForKeyValues(KeyValues* values) = 0;
	virtual void                  SOSSetOpvarFloat(const char*, float) = 0;
	virtual void                  SOSGetOpvarFloat(const char*, float&) = 0;
	virtual bool                  IsSubscribedMap(const char*, bool) = 0;
	virtual bool                  IsFeaturedMap(const char*, bool) = 0;
	virtual void                  GetDemoPlaybackParameters(void) = 0;
	virtual int                   GetClientVersion(void) = 0;
	virtual bool                  IsDemoSkipping(void) = 0;
	virtual void                  SetDemoImportantEventData(const KeyValues* values) = 0;
	virtual void                  ClearEvents(void) = 0;
	virtual int                   GetSafeZoneXMin(void) = 0;
	virtual bool                  IsVoiceRecording(void) = 0;
	virtual void                  ForceVoiceRecordOn(void) = 0;
	virtual bool                  IsReplay(void) = 0;
};
