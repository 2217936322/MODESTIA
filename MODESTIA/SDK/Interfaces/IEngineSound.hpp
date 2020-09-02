#pragma once

#include "../Math/Vector.hpp"
#include "../Misc/UtlVector.hpp"

struct SndInfo_t;
class IRecipientFilter;

typedef void* FileNameHandle_t;

struct SndInfo_t 
{
	int m_nGuid;
	FileNameHandle_t m_filenameHandle;
	int m_nSoundSource;
	int m_nChannel;
	int m_nSpeakerEntity;
	float m_flVolume;
	float m_flLastSpatializedVolume;
	float m_flRadius;
	int m_nPitch;
	Vector* m_pOrigin;
	Vector* m_pDirection;
	bool m_bUpdatePositions;
	bool m_bIsSentence;
	bool m_bDryMix;
	bool m_bSpeaker;
	bool m_bSpecialDSP;
	bool m_bFromServer;
};

#define	PITCH_NORM		100			  
#define PITCH_LOW		95			    
#define PITCH_HIGH		120

class IEngineSound
{
public:
	virtual bool	PrecacheSound(const char *pSample, bool bPreload = false, bool bIsUISound = false) = 0;
	virtual bool	IsSoundPrecached(const char *pSample) = 0;
	virtual void	PrefetchSound(const char *pSample) = 0;
	virtual bool	IsLoopingSound(const char *pSample) = 0;
	virtual float	GetSoundDuration(const char *pSample) = 0;
	virtual int		EmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;
	virtual int		EmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, SoundLevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;
	virtual void	EmitSentenceByIndex(IRecipientFilter& filter, int iEntIndex, int iChannel, int iSentenceIndex, float flVolume, SoundLevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;
	virtual void    StopSound(int iEntIndex, int iChannel, const char *pSample, unsigned int nSoundEntryHash) = 0;
	virtual void    StopAllSounds(bool bClearBuffers) = 0;
	virtual void    SetRoomType(IRecipientFilter& filter, int roomType) = 0;
	virtual void    SetPlayerDSP(IRecipientFilter& filter, int dspType, bool fastReset) = 0;
	virtual int     EmitAmbientSound(const char *pSample, float flVolume, int iPitch = PITCH_NORM, int flags = 0, float soundtime = 0.0f) = 0;
	virtual float   GetDistGainFromSoundLevel(SoundLevel_t soundlevel, float dist) = 0;
	virtual int		GetGuidForLastSoundEmitted() = 0;
	virtual bool	IsSoundStillPlaying(int guid) = 0;
	virtual void	StopSoundByGuid(int guid, bool bForceSync) = 0;
	virtual void	SetVolumeByGuid(int guid, float fvol) = 0;
	virtual void    unk() = 0;
	virtual void	GetActiveSounds(CUtlVector<SndInfo_t>& sndlist) = 0;
	virtual void	PrecacheSentenceGroup(const char *pGroupName) = 0;
	virtual void	NotifyBeginMoviePlayback() = 0;
	virtual void	NotifyEndMoviePlayback() = 0;
	virtual bool	GetSoundChannelVolume(const char* sound, float &flVolumeLeft, float &flVolumeRight) = 0;
	virtual float	GetElapsedTimeByGuid(int guid) = 0;
};
