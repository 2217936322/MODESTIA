#pragma once
#include "../SDK/CSGOStructs.hpp"

namespace EnginePrediction
{
	float curtimeBackup;
	float frametimeBackup;
	CMoveData MoveData;
	CUserCmd* PrevCmd;
	int fixedTick;

	int32_t* predictionSeed;
	C_BasePlayer*** predictionPlayer;

	void Begin( CUserCmd* cmd ) {
		curtimeBackup = g_GlobalVars->curtime;
		frametimeBackup = g_GlobalVars->frametime;

		if( !PrevCmd || PrevCmd->hasbeenpredicted ) {
			fixedTick = g_LocalPlayer->m_nTickBase( );
		} else {
			fixedTick++;
		}

		if( !predictionSeed || !predictionPlayer) {
			auto client = GetModuleHandle(TEXT("client.dll"));

			predictionSeed = *( int32_t** )( Utils::PatternScan( client, "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) + 0x2 );
			predictionPlayer = (C_BasePlayer*** )(Utils::PatternScan( client, "89 35 ? ? ? ? F3 0F 10 48 20" ) + 0x2);
		}
		
		if(predictionSeed) {
			*predictionSeed = cmd->random_seed;
		}

		if(predictionPlayer) {
			**predictionPlayer = g_LocalPlayer;
		}

		g_LocalPlayer->m_pCurrentCommand( ) = cmd;

		g_GlobalVars->curtime = static_cast< float >(fixedTick) * g_GlobalVars->interval_per_tick;
		g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;

		bool _inpred_backup = *( bool* )( ( uintptr_t )g_Prediction + 0x8 );

		*( bool* )( ( uintptr_t )g_Prediction + 0x8 ) = true;

		g_MoveHelper->SetHost( g_LocalPlayer );

		g_GameMovement->StartTrackPredictionErrors( g_LocalPlayer );
		g_Prediction->SetupMove( g_LocalPlayer, cmd, g_MoveHelper, & MoveData );
		g_GameMovement->ProcessMovement( g_LocalPlayer, & MoveData );
		g_Prediction->FinishMove( g_LocalPlayer, cmd, & MoveData );
		g_GameMovement->FinishTrackPredictionErrors( g_LocalPlayer );

		*( bool* )( ( uintptr_t )g_Prediction + 0x8 ) = _inpred_backup;

		g_MoveHelper->SetHost( nullptr );

		if(predictionSeed) {
			*predictionSeed = -1;
		}

		if(predictionPlayer) {
			**predictionPlayer = nullptr;
		}

		g_LocalPlayer->m_pCurrentCommand( ) = nullptr;

		PrevCmd = cmd;
	}

	void End( ) {
		g_GlobalVars->curtime = curtimeBackup;
		g_GlobalVars->frametime = frametimeBackup;
	}
}
