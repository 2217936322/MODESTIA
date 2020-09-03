#include "EnginePrediction.hpp"

#include "../Helpers/Memory.hpp"

static int localPlayerFlags;
CUserCmd* prevCmd;
CMoveData moveData;
int fixedTick;

void EnginePrediction::Run(CUserCmd* cmd)
{
    if (!g_LocalPlayer)
        return;

    localPlayerFlags = g_LocalPlayer->m_fFlags();

    *g_Memory.PredictionRandomSeed = 0;

    const auto curtimeBackup = g_GlobalVars->m_fCurtime;
    const auto frametimeBackup = g_GlobalVars->m_fFrametime;

    if (!prevCmd || prevCmd->m_bHasBeenPredicted)
    {
        fixedTick = g_LocalPlayer->m_nTickBase();
    }
    else
    {
        fixedTick++;
    }

    g_GlobalVars->m_fCurtime = static_cast<float>(fixedTick) * g_GlobalVars->m_fIntervalPerTick;
    g_GlobalVars->m_fFrametime = g_GlobalVars->m_fIntervalPerTick;

    g_MoveHelper->SetHost(g_LocalPlayer);
    g_Prediction->SetupMove(g_LocalPlayer, cmd, g_MoveHelper, &moveData);
    g_GameMovement->ProcessMovement(g_LocalPlayer, &moveData);
    g_Prediction->FinishMove(g_LocalPlayer, cmd, &moveData);
    g_MoveHelper->SetHost(nullptr);

    *g_Memory.PredictionRandomSeed = -1;

    g_GlobalVars->m_fCurtime = curtimeBackup;
    g_GlobalVars->m_fFrametime = frametimeBackup;
}

int EnginePrediction::GetFlags()
{
    return localPlayerFlags;
}