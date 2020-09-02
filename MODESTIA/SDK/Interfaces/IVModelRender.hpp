#pragma once

#include "../Math/QAngle.hpp"
#include "../Math/VMatrix.hpp"

#ifndef CONCAT_IMPL
#define CONCAT_IMPL(x, y) x##y
#ifndef MACRO_CONCAT
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#ifndef PAD
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
#endif
#endif
#endif
class KeyValues2 
{
public:
    PAD(32)
};

typedef unsigned short ModelInstanceHandle_t;
typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int OverrideType_t;
typedef int ImageFormat;
typedef int VertexFormat_t;
typedef int MaterialPropertyTypes_t;

class IClientRenderable;
class IMaterial;
class CStudioHdr;
class IMatRenderContext;
class DataCacheHandle_t;
class ITexture;
class IMaterialVar;
class KeyValues;
struct Model_t;
struct StudioAnimDesc_t;
struct StudioSeqDesc_t;
struct Ray_t;
struct DrawModelInfo_t;
struct StudioHwData_t;
struct MaterialLightingState_t;
struct ColorMeshInfo_t;

struct DrawModelState_t
{
    StudioHdr_t*            m_pStudioHdr;
    StudioHwData_t*         m_pStudioHWData;
    IClientRenderable*      m_pRenderable;
    const Matrix3x4_t*      m_pModelToWorld;
    StudioDecalHandle_t     m_decals;
    int                     m_drawFlags;
    int                     m_lod;
};

struct StaticPropRenderInfo_t
{
    const Matrix3x4_t*      pModelToWorld;
    const Model_t*          pModel;
    IClientRenderable*      pRenderable;
    Vector*                 pLightingOrigin;
    short                   skin;
    ModelInstanceHandle_t   instance;
};

struct ModelRenderInfo_t
{
    Vector                  origin;
    QAngle                  angles;
	char                    pad[4];
    IClientRenderable*      pRenderable;
    const Model_t*          pModel;
    const Matrix3x4_t*      pModelToWorld;
    const Matrix3x4_t*      pLightingOffset;
    const Vector*           pLightingOrigin;
    int                     flags;
    int                     entity_index;
    int                     skin;
    int                     body;
    int                     hitboxset;
    ModelInstanceHandle_t   instance;

    ModelRenderInfo_t()
    {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};

struct LightingQuery_t
{
    Vector                  m_LightingOrigin;
    ModelInstanceHandle_t   m_InstanceHandle;
    bool                    m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
    IClientRenderable*        m_pRenderable;
};

class IMaterial
{
public:
    const char* GetName()
    {
        using Fn = const char* (__thiscall*)(void*);
        return CallVirtualFunction<Fn>(this, 0)(this);
    }

    const char* GetTextureGroupName()
    {
        using Fn = const char* (__thiscall*)(void*);
        return CallVirtualFunction<Fn>(this, 1)(this);
    }

    void AlphaModulate(float a)
    {
        using Fn = void(__thiscall*)(void*, float);
        return CallVirtualFunction<Fn>(this, 27)(this, a);
    }

    void ColorModulate(float r, float g, float b)
    {
        using Fn = void(__thiscall*)(void*, float, float, float);
        return CallVirtualFunction<Fn>(this, 28)(this, r, g, b);
    }
};

class IVModelRender
{
public:
    virtual int                     DrawModel(int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index, const Model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const Matrix3x4_t *modelToWorld = NULL, const Matrix3x4_t *pLightingOffset = NULL) = 0;
    virtual void                    ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) = 0;
    virtual bool                    IsForcedMaterialOverride(void) = 0;
    virtual void                    SetViewTarget(const CStudioHdr *pStudioHdr, int nBodyIndex, const Vector& target) = 0;
    virtual ModelInstanceHandle_t   CreateInstance(IClientRenderable *pRenderable, LightCacheHandle_t *pCache = NULL) = 0;
    virtual void                    DestroyInstance(ModelInstanceHandle_t handle) = 0;
    virtual void                    SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
    virtual LightCacheHandle_t      GetStaticLighting(ModelInstanceHandle_t handle) = 0;
    virtual bool                    ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable *pRenderable) = 0;
    virtual void                    AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
    virtual void                    RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
    virtual bool                    ModelHasDecals(ModelInstanceHandle_t handle) = 0;
    virtual void                    RemoveAllDecalsFromAllModels() = 0;
    virtual Matrix3x4_t*            DrawModelShadowSetup(IClientRenderable *pRenderable, int body, int skin, DrawModelInfo_t *pInfo, Matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
    virtual void                    DrawModelShadow(IClientRenderable *pRenderable, const DrawModelInfo_t &info, Matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
    virtual bool                    RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
    virtual void                    ReleaseAllStaticPropColorData(void) = 0;
    virtual void                    RestoreAllStaticPropColorData(void) = 0;
    virtual int                     DrawModelEx(ModelRenderInfo_t &pInfo) = 0;
    virtual int                     DrawModelExStaticProp(ModelRenderInfo_t &pInfo) = 0;
    virtual bool                    DrawModelSetup(ModelRenderInfo_t &pInfo, DrawModelState_t *pState, Matrix3x4_t **ppBoneToWorldOut) = 0;
    virtual void                    DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, Matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
    virtual void                    SetupLighting(const Vector &vecCenter) = 0;
    virtual int                     DrawStaticPropArrayFast(StaticPropRenderInfo_t *pProps, int count, bool bShadowDepth) = 0;
    virtual void                    SuppressEngineLighting(bool bSuppress) = 0;
    virtual void                    SetupColorMeshes(int nTotalVerts) = 0;
    virtual void                    SetupLightingEx(const Vector &vecCenter, ModelInstanceHandle_t handle) = 0;
    virtual bool                    GetBrightestShadowingLightSource(const Vector &vecCenter, Vector& lightPos, Vector& lightBrightness, bool bAllowNonTaggedLights) = 0;
    virtual void                    ComputeLightingState(int nCount, const LightingQuery_t *pQuery, MaterialLightingState_t *pState, ITexture **ppEnvCubemapTexture) = 0;
    virtual void                    GetModelDecalHandles(StudioDecalHandle_t *pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t *pHandles) = 0;
    virtual void                    ComputeStaticLightingState(int nCount, const StaticLightingQuery_t *pQuery, MaterialLightingState_t *pState, MaterialLightingState_t *pDecalState, ColorMeshInfo_t **ppStaticLighting, ITexture **ppEnvCubemapTexture, DataCacheHandle_t *pColorMeshHandles) = 0;
    virtual void                    CleanupStaticLightingState(int nCount, DataCacheHandle_t *pColorMeshHandles) = 0;
};
