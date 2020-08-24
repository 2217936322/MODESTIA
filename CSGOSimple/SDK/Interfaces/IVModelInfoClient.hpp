#pragma once

#include "IEngineTrace.hpp"

#include "../Math/QAngle.hpp"
#include "../Misc/Studio.hpp"

class CPhysCollide;
class CUtlBuffer;
class IClientRenderable;
class CStudioHdr;
struct VirtualModel_t;

class IVModelInfo
{
public:
	virtual                                 ~IVModelInfo(void) {}
	virtual const Model_t*					GetModel(int modelIndex) const = 0;
	virtual int                             GetModelIndex(const char* name) const = 0;
	virtual const char*						GetModelName(const Model_t* model) const = 0;
	virtual VCollide_t*						GetVCollide(const Model_t* model) const = 0;
	virtual VCollide_t*						GetVCollide(int modelIndex) const = 0;
	virtual void                            GetModelBounds(const Model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual void                            GetModelRenderBounds(const Model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual int                             GetModelFrameCount(const Model_t* model) const = 0;
	virtual int                             GetModelType(const Model_t* model) const = 0;
	virtual void*							GetModelExtraData(const Model_t* model) = 0;
	virtual bool                            ModelHasMaterialProxy(const Model_t* model) const = 0;
	virtual bool                            IsTranslucent(Model_t const* model) const = 0;
	virtual bool                            IsTranslucentTwoPass(const Model_t* model) const = 0;
	virtual void                            Unused0() {};
	virtual void							UNUSED() = 0;
	virtual void							UNUSE11D() = 0;
	virtual RenderableTranslucencyType_t    ComputeTranslucencyType(const Model_t* model, int skin, int body) = 0;
	virtual int                             GetModelMaterialCount(const Model_t* model) const = 0;
	virtual void                            GetModelMaterials(const Model_t* model, int count, IMaterial** material) = 0;
	virtual bool                            IsModelVertexLit(const Model_t* model) const = 0;
	virtual const char*						GetModelKeyValueText(const Model_t* model) = 0;
	virtual bool                            GetModelKeyValue(const Model_t* model, CUtlBuffer& buf) = 0;
	virtual float                           GetModelRadius(const Model_t* model) = 0;
	virtual CStudioHdr*						GetStudioHdr(MDLHandle_t handle) = 0;
	virtual const StudioHdr_t*				FindModel(const StudioHdr_t* studioHdr, void** cache, const char* modelname) const = 0;
	virtual const StudioHdr_t*				FindModel(void* cache) const = 0;
	virtual VirtualModel_t*					GetVirtualModel(const StudioHdr_t* studioHdr) const = 0;
	virtual uint8_t*						GetAnimBlock(const StudioHdr_t* studioHdr, int block) const = 0;
	virtual void                            GetModelMaterialColorAndLighting(const Model_t* model, Vector const& origin, QAngle const& angles, Trace_t* trace, Vector& lighting, Vector& matColor) = 0;
	virtual void                            GetIlluminationPoint(const Model_t* model, IClientRenderable* renderable, Vector const& origin, QAngle const& angles, Vector* lightingCenter) = 0;
	virtual int                             GetModelContents(int modelIndex) const = 0;
	virtual StudioHdr_t*					GetStudiomodel(const Model_t* mod) = 0;
	virtual int                             GetModelSpriteWidth(const Model_t* model) const = 0;
	virtual int                             GetModelSpriteHeight(const Model_t* model) const = 0;
	virtual void                            SetLevelScreenFadeRange(float minSize, float maxSize) = 0;
	virtual void                            GetLevelScreenFadeRange(float* minArea, float* maxArea) const = 0;
	virtual void                            SetViewScreenFadeRange(float minSize, float maxSize) = 0;
	virtual unsigned char                   ComputeLevelScreenFade(const Vector& absOrigin, float radius, float fadeScale) const = 0;
	virtual unsigned char                   ComputeViewScreenFade(const Vector& absOrigin, float radius, float fadeScale) const = 0;
	virtual int                             GetAutoplayList(const StudioHdr_t* studioHdr, unsigned short** autoPlaylist) const = 0;
	virtual CPhysCollide*					GetCollideForVirtualTerrain(int index) = 0;
	virtual bool                            IsUsingFBTexture(const Model_t* model, int skin, int body, IClientRenderable** clientRenderable) const = 0;
	virtual const Model_t*					FindOrLoadModel(const char* name) const = 0;
	virtual MDLHandle_t                     GetCacheHandle(const Model_t* model) const = 0;
	virtual int                             GetBrushModelPlaneCount(const Model_t* model) const = 0;
	virtual void                            GetBrushModelPlane(const Model_t* model, int index, CPlane_t& plane, Vector* origin) const = 0;
	virtual int                             GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool                            UsesEnvCubemap(const Model_t* model) const = 0;
	virtual bool                            UsesStaticLighting(const Model_t* model) const = 0;
};

class IVModelInfoClient : public IVModelInfo
{
public:
};
