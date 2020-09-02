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

    const auto curtimeBackup = g_GlobalVars->curtime;
    const auto frametimeBackup = g_GlobalVars->frametime;

    if (!prevCmd || prevCmd->m_bHasBeenPredicted)
    {
        fixedTick = g_LocalPlayer->m_nTickBase();
    }
    else
    {
        fixedTick++;
    }

    g_GlobalVars->curtime = static_cast<float>(fixedTick) * g_GlobalVars->interval_per_tick;
    g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;

    g_MoveHelper->SetHost(g_LocalPlayer);
    g_Prediction->SetupMove(g_LocalPlayer, cmd, g_MoveHelper, &moveData);
    g_GameMovement->ProcessMovement(g_LocalPlayer, &moveData);
    g_Prediction->FinishMove(g_LocalPlayer, cmd, &moveData);
    g_MoveHelper->SetHost(nullptr);

    *g_Memory.PredictionRandomSeed = -1;

    g_GlobalVars->curtime = curtimeBackup;
    g_GlobalVars->frametime = frametimeBackup;
}

int EnginePrediction::GetFlags()
{
    return localPlayerFlags;
}