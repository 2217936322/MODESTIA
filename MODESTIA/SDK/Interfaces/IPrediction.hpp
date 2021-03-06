#pragma once

#include "../Math/QAngle.hpp"
#include "../Misc/CUserCmd.hpp"
#include "IMoveHelper.hpp"

class CMoveData
{
public:
    bool    m_bFirstRunOfFunctions : 1;
    bool    m_bGameCodeMovedPlayer : 1;
    int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
    int     m_nImpulseCommand;      // Impulse command issued.
    Vector  m_vecViewAngles;        // Command view angles (local space)
    Vector  m_vecAbsViewAngles;     // Command view angles (world space)
    int     m_nButtons;             // Attack buttons.
    int     m_nOldButtons;          // From host_client->oldbuttons;
    float   m_flForwardMove;
    float   m_flSideMove;
    float   m_flUpMove;
    float   m_flMaxSpeed;
    float   m_flClientMaxSpeed;
    Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
    Vector  m_vecAngles;            // edict::angles
    Vector  m_vecOldAngles;
    float   m_outStepHeight;        // how much you climbed this move
    Vector  m_outWishVel;           // This is where you tried 
    Vector  m_outJumpVel;           // This is your jump velocity
    Vector  m_vecConstraintCenter;
    float   m_flConstraintRadius;
    float   m_flConstraintWidth;
    float   m_flConstraintSpeedFactor;
    float   m_flUnknown[5];
    Vector  m_vecAbsOrigin;
};

class CBasePlayer;

class IGameMovement
{
public:
    virtual			~IGameMovement(void) {}

    virtual void	          ProcessMovement(CBasePlayer *pPlayer, CMoveData *pMove) = 0;
    virtual void	          Reset(void) = 0;
    virtual void	          StartTrackPredictionErrors(CBasePlayer *pPlayer) = 0;
    virtual void	          FinishTrackPredictionErrors(CBasePlayer *pPlayer) = 0;
    virtual void	          DiffPrint(char const *fmt, ...) = 0;
    virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
    virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
    virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
    virtual bool		        IsMovingPlayerStuck(void) const = 0;
    virtual CBasePlayer*   GetMovingPlayer(void) const = 0;
    virtual void		        UnblockPusher(CBasePlayer *pPlayer, CBasePlayer *pPusher) = 0;
    virtual void            SetupMovementBounds(CMoveData *pMove) = 0;
};

class CGameMovement
    : public IGameMovement
{
public:
    virtual ~CGameMovement(void) {}
};

class IPrediction
{
public:
    bool InPrediction()
    {
        typedef bool(__thiscall* oInPrediction)(void*);
        return CallVirtualFunction<oInPrediction>(this, 14)(this);
    }

    void RunCommand(CBasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
    {
        typedef void(__thiscall* oRunCommand)(void*, CBasePlayer*, CUserCmd*, IMoveHelper*);
        return CallVirtualFunction<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
    }

    void SetupMove(CBasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData)
    {
        typedef void(__thiscall* oSetupMove)(void*, CBasePlayer*, CUserCmd*, IMoveHelper*, void*);
        return CallVirtualFunction<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
    }

    void FinishMove(CBasePlayer *player, CUserCmd *ucmd, void*pMoveData)
    {
        typedef void(__thiscall* oFinishMove)(void*, CBasePlayer*, CUserCmd*, void*);
        return CallVirtualFunction<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
    }
};