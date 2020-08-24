#pragma once

class LightDesc_t;
class LightPos_t;

class LightDesc_t 
{
public:
  LightType_t m_Type;                 
  Vector m_Color;                     
  Vector m_Position;                  
  Vector m_Direction;                 
  float m_Range;                      
  float m_Falloff;                    
  float m_Attenuation0;               
  float m_Attenuation1;               
  float m_Attenuation2;               
  float m_Theta;                      
  float m_Phi;                        
  float m_ThetaDot;                   
  float m_PhiDot;                     
  float m_OneOverThetaDotMinusPhiDot; 
  __int32 m_Flags;                    
  float m_RangeSquared;               

}; 

class LightPos_t 
{
public:
  Vector delta;  
  float falloff; 
  float dot;     
}; 

struct MaterialLightingState_t 
{
  Vector m_vecAmbientCube[6]; 
  Vector m_vecLightingOrigin;  
  int m_nLocalLightCount;
  LightDesc_t m_pLocalLightDesc[4];
};

struct DrawModelInfo_t 
{
  StudioHdr_t* m_pStudioHdr;
  StudioHwData_t* m_pHardwareData;
  StudioDecalHandle_t m_Decals;
  int m_Skin;
  int m_Body;
  int m_HitboxSet;
  IClientRenderable* m_pClientEntity;
  int m_Lod;
  ColorMeshInfo_t* m_pColorMeshes;
  bool m_bStaticLighting;
  MaterialLightingState_t m_LightingState;
};

class IStudioRender 
{
public:
  void* vtable;
  float fEyeShiftX;                      
  float fEyeShiftY;                      
  float fEyeShiftZ;                      
  float fEyeSize;                        
  float fEyeGlintPixelWidthLODThreshold; 
  __int32 maxDecalsPerModel;             
  __int32 drawEntities;                  
  __int32 skin;                          
  __int32 fullbright;                    
  bool bEyeMove : 1;                       
  bool bSoftwareSkin : 1;
  bool bNoHardware : 1;
  bool bNoSoftware : 1;
  bool bTeeth : 1;
  bool bEyes : 1;
  bool bFlex : 1;
  bool bWireframe : 1;
  bool bDrawNormals : 1;
  bool bDrawTangentFrame : 1;
  bool bDrawZBufferedWireframe : 1;
  bool bSoftwareLighting : 1;
  bool bShowEnvCubemapOnly : 1;
  bool bWireframeDecals : 1;
  int m_nReserved[5];
  Vector m_ViewTarget;           
  Vector m_ViewOrigin;           
  Vector m_ViewRight;            
  Vector m_ViewUp;               
  Vector m_ViewPlaneNormal;      
  Vector4D m_LightBoxColors[6];  
  LightDesc_t m_LocalLights[4];  
  __int32 m_NumLocalLights;      
  float m_ColorMod[3];           
  float m_AlphaMod;              
  IMaterial* m_pForcedMaterial;  
  __int32 m_nForcedMaterialType; 
  char pad_0x0258[0xC];          
  __int32 unkhandle1;            
  __int32 unkhandle2;            
  __int32 unkhandle3;            
  __int32 unkhandle4;            
  char pad_0x0274[0x4];          
  LightPos_t m_pLightPos[16];    
};

static_assert(FIELD_OFFSET(IStudioRender, m_pForcedMaterial) == 0x250, "Wrong m_pForcedMaterial offset");
