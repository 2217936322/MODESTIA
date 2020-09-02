#pragma once

#include "../SDK.hpp"

class LightDesc_t 
{
public:
	LightType_t m_pType;                 
	Vector m_vecColor;                     
	Vector m_vecPosition;                  
	Vector m_vecDirection;                 
	float m_fRange;                      
	float m_fFalloff;                    
	float m_fAttenuation0;               
	float m_fAttenuation1;               
	float m_fAttenuation2;               
	float m_fTheta;                      
	float m_fPhi;                        
	float m_fThetaDot;                   
	float m_fPhiDot;                     
	float m_fOneOverThetaDotMinusPhiDot; 
	__int32 m_iFlags;                    
	float m_fRangeSquared;               
}; 

class LightPos_t 
{
public:
	Vector m_vecDelta;  
	float m_fFallOff; 
	float m_fDot;     
}; 

struct MaterialLightingState_t 
{
	Vector m_vecAmbientCube[6]; 
	Vector m_vecLightingOrigin;  
	int m_iLocalLightCount;
	LightDesc_t m_pLocalLightDesc[4];
};

struct DrawModelInfo_t 
{
	StudioHdr_t* m_pStudioHdr;
	StudioHwData_t* m_pHardwareData;
	StudioDecalHandle_t m_pDecals;
	int m_iSkin;
	int m_iBody;
	int m_iHitboxSet;
	IClientRenderable* m_pClientEntity;
	int m_iLod;
	ColorMeshInfo_t* m_pColorMeshes;
	bool m_bStaticLighting;
	MaterialLightingState_t m_pLightingState;
};

class IStudioRender {
public:
	void* m_vTable;
	float m_fEyeShiftX;                      
	float m_fEyeShiftY;                      
	float m_fEyeShiftZ;
	float m_fEyeSize;
	float m_fEyeGlintPixelWidthLODThreshold;
	__int32 m_iMaxDecalsPerModel;
	__int32 m_iDrawEntities;
	__int32 m_iSkin;
	__int32 m_iFullbright;
	bool m_bEyeMove : 1;
	bool m_bSoftwareSkin : 1;
	bool m_bNoHardware : 1;
	bool m_bNoSoftware : 1;
	bool m_bTeeth : 1;
	bool m_bEyes : 1;
	bool m_bFlex : 1;
	bool m_bWireframe : 1;
	bool m_bDrawNormals : 1;
	bool m_bDrawTangentFrame : 1;
	bool m_bDrawZBufferedWireframe : 1;
	bool m_bSoftwareLighting : 1;
	bool m_bShowEnvCubemapOnly : 1;
	bool m_bWireframeDecals : 1;
	int m_iReserved[5];
	Vector m_vecViewTarget;           
	Vector m_vecViewOrigin;           
	Vector m_vecViewRight;            
	Vector m_vecViewUp;               
	Vector m_vecViewPlaneNormal;      
	Vector4D m_vec4LightBoxColors[6];  
	LightDesc_t m_pLocalLights[4];  
	__int32 m_iNumLocalLights;      
	float m_fColorMod[3];           
	float m_fAlphaMod;              
	IMaterial* m_pForcedMaterial;  
	__int32 m_iForcedMaterialType; 
	char pad_0x0258[0xC];          
	__int32 unkhandle1;            
	__int32 unkhandle2;           
	__int32 unkhandle3;          
	__int32 unkhandle4;            
	char pad_0x0274[0x4];         
	LightPos_t m_pLightPos[16];    
};

static_assert(FIELD_OFFSET(IStudioRender, m_pForcedMaterial) == 0x250, "Wrong m_pForcedMaterial offset");
