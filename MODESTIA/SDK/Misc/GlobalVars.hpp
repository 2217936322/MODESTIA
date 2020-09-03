#pragma once

class CGlobalVarsBase
{
public:
    float     m_fRealtime;                      
    int       m_iFramecount;                    
    float     m_fAbsoluteFrametime;             
    float     m_fAbsoluteFrameStartTime;  
    float     m_fCurtime;                       
    float     m_fFrametime;                     
    int       m_iMaxClients;                    
    int       m_iTickcount;                     
    float     m_fIntervalPerTick;             
    float     m_fInterpolationAmount;          
    int       m_iSimTicksThisFrame;             
    int       m_iNetworkProtocol;              
    void*     m_pSaveData;                     
    bool      m_bClient;                     
    bool      m_bRemoteClient;               

private:
    int       m_iTimestampNetworkingBase;
    int       m_iTimestampRandomizeWindow;
};