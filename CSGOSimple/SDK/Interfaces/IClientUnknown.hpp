#pragma once

#include "../Misc/IHandleEntity.hpp"
#include "ICollideable.hpp"

class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class CBaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

class IClientUnknown : public IHandleEntity
{
public:
    virtual ICollideable*              GetCollideable() = 0;
    virtual IClientNetworkable*        GetClientNetworkable() = 0;
    virtual IClientRenderable*         GetClientRenderable() = 0;
    virtual IClientEntity*             GetIClientEntity() = 0;
    virtual CBaseEntity*               GetBaseEntity() = 0;
    virtual IClientThinkable*          GetClientThinkable() = 0;
    virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};