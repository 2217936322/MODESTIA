#pragma once

#include "../math/Vector4D.hpp"

#pragma region MASKS
#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40 
#define   CONTENTS_OPAQUE               0x80 
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200     
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800 
#define   CONTENTS_TEAM2                0x1000 
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000 

#define   CONTENTS_MONSTER              0x2000000 
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000 
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001 
#define   SURF_SKY2D                    0x0002 
#define   SURF_SKY                      0x0004 
#define   SURF_WARP                     0x0008 
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020 
#define   SURF_TRIGGER                  0x0040 
#define   SURF_NODRAW                   0x0080 

#define   SURF_HINT                     0x0100 

#define   SURF_SKIP                     0x0200   
#define   SURF_NOLIGHT                  0x0400   
#define   SURF_BUMPLIGHT                0x0800   
#define   SURF_NOSHADOWS                0x1000   
#define   SURF_NODECALS                 0x2000   
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000   
#define   SURF_HITBOX                   0x8000   

#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)
#pragma endregion

class IHandleEntity;
struct Ray_t;
class CGameTrace;
typedef CGameTrace Trace_t;
class ICollideable;
class ITraceListData;
class CPhysCollide;
struct CPlane_t;

class ITraceFilter
{
public:
    virtual bool ShouldHitEntity(IHandleEntity* entity, int contentsMask) = 0;
    virtual TraceType GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
    bool ShouldHitEntity(IHandleEntity* entityHandle, int)
    {
        ClientClass* entity = ((IClientEntity*)entityHandle)->GetClientClass();
        if (entity && strcmp(ccIgnore, ""))
        {
            if (entity->m_pNetworkName == ccIgnore)
                return false;
        }

        return !(entityHandle == pSkip);
    }

    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_EVERYTHING;
    }

    inline void SetIgnoreClass(char* class_)
    {
        ccIgnore = class_;
    }

    void* pSkip;
    char* ccIgnore = "";
};

class CTraceFilterOneEntity : public CTraceFilter
{
public:
    bool ShouldHitEntity(IHandleEntity* entityHandle, int)
    {
        return (entityHandle == entity);
    }

    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_EVERYTHING;
    }

    void* entity;
};

class CTraceFilterSkientity : public ITraceFilter
{
public:
    CTraceFilterSkientity(IHandleEntity* entityHandle)
    {
        pSkip = entityHandle;
    }

    bool ShouldHitEntity(IHandleEntity* entityHandle, int)
    {
        return !(entityHandle == pSkip);
    }
    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_EVERYTHING;
    }
    void* pSkip;
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
    bool ShouldHitEntity(IHandleEntity* entityHandle, int)
    {
        return true;
    }
    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_ENTITIES_ONLY;
    }
};

class CTraceFilterWorldOnly : public ITraceFilter
{
public:
    bool ShouldHitEntity(IHandleEntity*, int)
    {
        return false;
    }

    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_WORLD_ONLY;
    }
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
    bool ShouldHitEntity(IHandleEntity*, int)
    {
        return false;
    }

    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_EVERYTHING;
    }
};

class CTraceFilterPlayersOnlySkipOne : public ITraceFilter
{
public:
    CTraceFilterPlayersOnlySkipOne(IClientEntity* ent)
    {
        pEnt = ent;
    }
    bool ShouldHitEntity(IHandleEntity* entityHandle, int)
    {
        return entityHandle != pEnt && ((IClientEntity*)entityHandle)->GetClientClass()->m_ClassID == C_CSPlayer;
    }
    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_ENTITIES_ONLY;
    }

private:
    IClientEntity* pEnt;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
    CTraceFilterSkipTwoEntities(IClientEntity* ent1, IClientEntity* ent2)
    {
        pEnt1 = ent1;
        pEnt2 = ent2;
    }
    bool ShouldHitEntity(IHandleEntity* entityHandle, int)
    {
        return !(entityHandle == pEnt1 || entityHandle == pEnt2);
    }
    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_EVERYTHING;
    }

private:
    IClientEntity* pEnt1;
    IClientEntity* pEnt2;
};

class CTraceFilterHitAll : public CTraceFilter
{
public:
    virtual bool ShouldHitEntity(IHandleEntity*, int)
    {
        return true;
    }
};

class IEntityEnumerator
{
public:
    virtual bool EnumEntity(IHandleEntity* handleEntity) = 0;
};

struct BrushSideInfo_t
{
    Vector4D plane;              
    unsigned short bevel;    
    unsigned short thin;     
};

class CPhysCollide;

struct VCollide_t
{
    unsigned short solidCount : 15;
    unsigned short isPacked : 1;
    unsigned short descSize;
    CPhysCollide** solids;
    char* pKeyValues;
    void* pUserData;
};

struct CModel_t
{
    Vector         mins, maxs;
    Vector         origin;       
    int            headnode;
    VCollide_t     vcollisionData;
};

struct CSurface_t
{
    const char* name;
    short          surfaceProps;
    unsigned short flags;         
};


struct Ray_t
{
    VectorAligned  m_Start; 
    VectorAligned  m_Delta; 
    VectorAligned  m_StartOffset; 
    VectorAligned  m_Extents;    
    const Matrix3x4_t* m_pWorldAxisTransform;
    bool m_IsRay; 
    bool m_IsSwept;     

    Ray_t() : m_pWorldAxisTransform(NULL) {}

    void Init(Vector const& start, Vector const& end)
    {
        m_Delta = end - start;

        m_IsSwept = (m_Delta.LengthSqr() != 0);

        m_Extents.Init();

        m_pWorldAxisTransform = NULL;
        m_IsRay = true;

        m_StartOffset.Init();
        m_Start = start;
    }

    void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
    {
        m_Delta = end - start;

        m_pWorldAxisTransform = NULL;
        m_IsSwept = (m_Delta.LengthSqr() != 0);

        m_Extents = maxs - mins;
        m_Extents *= 0.5f;
        m_IsRay = (m_Extents.LengthSqr() < 1e-6);
        m_StartOffset = maxs + mins;
        m_StartOffset *= 0.5f;
        m_Start = start + m_StartOffset;
        m_StartOffset *= -1.0f;
    }

    Vector InvDelta() const
    {
        Vector vecInvDelta;
        for (int axis = 0; axis < 3; ++axis) 
        {
            if (m_Delta[axis] != 0.0f) {
                vecInvDelta[axis] = 1.0f / m_Delta[axis];
            }
            else 
            {
                vecInvDelta[axis] = FLT_MAX;
            }
        }
        return vecInvDelta;
    }

private:
};

class CBaseTrace
{
public:
    bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
    bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
    bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
    bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
    bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
    Vector         startpos;           
    Vector         endpos;            
    CPlane_t       plane;               

    float          fraction;           

    int            contents;            
    unsigned short dispFlags;         

    bool           allsolid;            
    bool           startsolid;         

    CBaseTrace() {}

};

class CGameTrace : public CBaseTrace
{
public:
    bool DidHit() const;
    bool IsVisible() const;

public:

    float               fractionleftsolid;  
    CSurface_t          surface;           
    int                 hitgroup;          
    short               physicsbone;        
    unsigned short      worldSurfaceIndex;  
    IClientEntity* hit_entity;
    int                 hitbox;                     

    CGameTrace() {}

private:
    CGameTrace(const CGameTrace& other) :
        fractionleftsolid(other.fractionleftsolid),
        surface(other.surface),
        hitgroup(other.hitgroup),
        physicsbone(other.physicsbone),
        worldSurfaceIndex(other.worldSurfaceIndex),
        hit_entity(other.hit_entity),
        hitbox(other.hitbox)
    {
        startpos = other.startpos;
        endpos = other.endpos;
        plane = other.plane;
        fraction = other.fraction;
        contents = other.contents;
        dispFlags = other.dispFlags;
        allsolid = other.allsolid;
        startsolid = other.startsolid;
    }
};

inline bool CGameTrace::DidHit() const
{
    return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
    return fraction > 0.97f;
}

class IEngineTrace
{
public:
    virtual int   GetPointContents(const Vector& absPosition, int contentsMask = MASK_ALL, IHandleEntity** entity = nullptr) = 0;
    virtual int   GetPointContents_WorldOnly(const Vector& absPosition, int contentsMask = MASK_ALL) = 0;
    virtual int   GetPointContents_Collideable(ICollideable* collide, const Vector& absPosition) = 0;
    virtual void  ClipRayToEntity(const Ray_t& ray, unsigned int mask, IHandleEntity* entity, CGameTrace* trace) = 0;
    virtual void  ClipRayToCollideable(const Ray_t& ray, unsigned int mask, ICollideable* collide, CGameTrace* trace) = 0;
    virtual void  TraceRay(const Ray_t& ray, unsigned int mask, ITraceFilter* traceFilter, CGameTrace* trace) = 0;
};