#pragma once

#include "../Misc/CUserCmd.hpp"

#define MULTIPLAYER_BACKUP 150

class CInput
{
public:
	char                pad_0x00[0x0C];  
	bool                m_trackir_available;  
	bool                m_mouse_initiated;  
	bool                m_mouse_active;  
	bool                m_fJoystickAdvancedInit;       
	char                pad_0x08[0x2C];                
	void*               m_pKeys;                       
	char                pad_0x38[0x6C];                
	bool                m_fCameraInterceptingMouse;    
	bool                m_fCameraInThirdPerson;        
	bool                m_fCameraMovingWithMouse;      
	Vector				m_vecCameraOffset;             
	bool                m_fCameraDistanceMove;         
	int                 m_nCameraOldX;                 
	int                 m_nCameraOldY;                 
	int                 m_nCameraX;                    
	int                 m_nCameraY;                    
	bool                m_CameraIsOrthographic;        
	Vector              m_angPreviousViewAngles;       
	Vector              m_angPreviousViewAnglesTilt;   
	float               m_flLastForwardMove;           
	int                 m_nClearInputState;            
	char                pad_0xE4[0x8];                 
	CUserCmd*           m_pCommands;                   
	CVerifiedUserCmd*   m_pVerifiedCommands;           

	inline CUserCmd* GetUserCmd(int sequenceNumber);
	inline CUserCmd * GetUserCmd(int slot, int sequenceNumber);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequenceNumber);
};

CUserCmd* CInput::GetUserCmd(int sequenceNumber)
{
	using OriginalFn = CUserCmd * (__thiscall*)(void*, int, int);
	return CallVFunction<OriginalFn>(this, 8)(this, 0, sequenceNumber);
}

CUserCmd* CInput::GetUserCmd(int slot, int sequenceNumber)
{
	typedef CUserCmd* (__thiscall* GetUserCmd_t)(void*, int, int);
	return CallVFunction<GetUserCmd_t>(this, 8)(this, slot, sequenceNumber);
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequenceNumber)
{
	auto verifiedCommands = *(CVerifiedUserCmd**)(reinterpret_cast<uint32_t>(this) + 0xF8);
	return &verifiedCommands[sequenceNumber % MULTIPLAYER_BACKUP];
}
