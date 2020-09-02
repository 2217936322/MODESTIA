#pragma once

#include "ChecksumCrc.hpp"
#include "ChecksumMd5.hpp"
#include "../Math/QAngle.hpp"

#define IN_ATTACK       (1 << 0)
#define IN_JUMP         (1 << 1)
#define IN_DUCK         (1 << 2)
#define IN_FORWARD      (1 << 3)
#define IN_BACK         (1 << 4)
#define IN_USE          (1 << 5)
#define IN_CANCEL       (1 << 6)
#define IN_LEFT         (1 << 7)
#define IN_RIGHT        (1 << 8)
#define IN_MOVELEFT     (1 << 9)
#define IN_MOVERIGHT    (1 << 10)
#define IN_ATTACK2      (1 << 11)
#define IN_RUN          (1 << 12)
#define IN_RELOAD       (1 << 13)
#define IN_ALT1         (1 << 14)
#define IN_ALT2         (1 << 15)
#define IN_SCORE        (1 << 16)  
#define IN_SPEED        (1 << 17) 
#define IN_WALK         (1 << 18) 
#define IN_ZOOM         (1 << 19) 
#define IN_WEAPON1      (1 << 20) 
#define IN_WEAPON2      (1 << 21) 
#define IN_BULLRUSH     (1 << 22)
#define IN_GRENADE1     (1 << 23) 
#define IN_GRENADE2     (1 << 24) 
#define IN_LOOKSPIN     (1 << 25)

class CUserCmd
{
public:
    CUserCmd()
    {
        memset(this, 0, sizeof(*this));
    };
    virtual ~CUserCmd() {};

    CRC32_t GetChecksum(void) const
    {
        CRC32_t crc;
        CRC32_Init(&crc);

        CRC32_ProcessBuffer(&crc, &m_iCommandNumber, sizeof(m_iCommandNumber));
        CRC32_ProcessBuffer(&crc, &m_iTickCount, sizeof(m_iTickCount));
        CRC32_ProcessBuffer(&crc, &m_angViewangles, sizeof(m_angViewangles));
        CRC32_ProcessBuffer(&crc, &m_vecAimDirection, sizeof(m_vecAimDirection));
        CRC32_ProcessBuffer(&crc, &m_fForwardMove, sizeof(m_fForwardMove));
        CRC32_ProcessBuffer(&crc, &m_fSideMove, sizeof(m_fSideMove));
        CRC32_ProcessBuffer(&crc, &m_fUpMove, sizeof(m_fUpMove));
        CRC32_ProcessBuffer(&crc, &m_iButtons, sizeof(m_iButtons));
        CRC32_ProcessBuffer(&crc, &m_szImpulse, sizeof(m_szImpulse));
        CRC32_ProcessBuffer(&crc, &m_iWeaponSelect, sizeof(m_iWeaponSelect));
        CRC32_ProcessBuffer(&crc, &m_iWeaponSubType, sizeof(m_iWeaponSubType));
        CRC32_ProcessBuffer(&crc, &m_iRandomSeed, sizeof(m_iRandomSeed));
        CRC32_ProcessBuffer(&crc, &m_MouseDX, sizeof(m_MouseDX));
        CRC32_ProcessBuffer(&crc, &m_MouseDY, sizeof(m_MouseDY));

        CRC32_Final(&crc);
        return crc;
    }

    int     m_iCommandNumber;    
    int     m_iTickCount;         
    QAngle  m_angViewangles;         
    Vector  m_vecAimDirection;       
    float   m_fForwardMove;        
    float   m_fSideMove;          
    float   m_fUpMove;         
    int     m_iButtons;            
    char    m_szImpulse;        
    int     m_iWeaponSelect;       
    int     m_iWeaponSubType;    
    int     m_iRandomSeed;        
    short   m_MouseDX;          
    short   m_MouseDY;          
    bool    m_bHasBeenPredicted;   
    char    pad_0x4C[0x18];    
};

class CVerifiedUserCmd
{
public:
    CUserCmd m_Cmd;
    CRC32_t  m_CRC;
};
