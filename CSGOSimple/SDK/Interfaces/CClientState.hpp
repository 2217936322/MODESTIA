#pragma once

#include <cstdint>

#pragma pack(push, 1)
class INetChannel
{
public:
    char pad_0000[20];           
    bool m_bProcessingMessages;  
    bool m_bShouldDelete;       
    char pad_0016[2];      
    int32_t m_nOutSequenceNr;    
    int32_t m_nInSequenceNr;    
    int32_t m_nOutSequenceNrAck; 
    int32_t m_nOutReliableState;
    int32_t m_nInReliableState; 
    int32_t m_nChokedPackets;    
    char pad_0030[1044];         
}; 

class CClockDriftMgr
{
public:
    float m_ClockOffsets[16];  
    uint32_t m_iCurClockOffset;
    uint32_t m_nServerTick;     
    uint32_t m_nClientTick;    
};

class CClientState
{
public:
	void ForceFullUpdate()
	{
		*reinterpret_cast<int*>(std::uintptr_t(this) + 0x174) = -1;
	}

	char			pad_0000[156];
	INetChannel*	m_NetChannel;
	uint32_t		m_nChallengeNr;
	char			pad_00A4[100];
	uint32_t		m_nSignonState;
	char			pad_010C[8];
	float			m_flNextCmdTime;
	uint32_t		m_nServerCount;
	uint32_t		m_nCurrentSequence;
	char			pad_0120[8];
	CClockDriftMgr	m_ClockDriftMgr;
	uint32_t		m_nDeltaTick;
	bool			m_bPaused;
	char			pad_017D[3];
	uint32_t		m_nViewEntity;
	uint32_t		m_nPlayerSlot;
	char			m_szLevelName[260];
	char			m_szLevelNameShort[80];
	char			m_szGroupName[80];
	char			pad_032Ñ[92];
	uint32_t		m_nMaxClients;
	char			pad_0314[18824];
	float			m_flLastServerTickTime;
	bool			insimulation;
	char			pad_4C9D[3];
	uint32_t		oldtickcount;
	float			m_tickRemainder;
	float			m_frameTime;
	int				lastoutgoingcommand;
	int				chokedcommands;
	int				last_command_ack;
	int				command_ack;
	int				m_nSoundSequence;
	char			pad_4CC0[80];
	QAngle			viewangles;
	char			pad_4D1C[208];
};
#pragma pack(pop)

static_assert( FIELD_OFFSET( CClientState, m_NetChannel ) == 0x009C, "Wrong struct offset" );
static_assert( FIELD_OFFSET( CClientState, m_nCurrentSequence ) == 0x011C, "Wrong struct offset" );
static_assert( FIELD_OFFSET( CClientState, m_nDeltaTick ) == 0x0174, "Wrong struct offset" );
