#pragma once

#include "../Math/Vector.hpp"

typedef float Quaternion[4];
typedef float RadianEuler[3];

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01   
#define BONE_PHYSICS_PROCEDURAL         0x02   
#define BONE_ALWAYS_PROCEDURAL          0x04    
#define BONE_SCREEN_ALIGN_SPHERE        0x08    
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    
#define BONE_USED_BY_ATTACHMENT         0x00000200    
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    
#define BONE_USED_BY_VERTEX_LOD1        0x00000800    
#define BONE_USED_BY_VERTEX_LOD2        0x00001000  
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS                    8
#define MAXSTUDIOBONES		            128		

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    

#define BONE_HAS_SAVEFRAME_POS          0x00200000  
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000   

#define Assert( _exp ) ((void)0)

#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4    
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR 10

enum ModType_t
{
    ModType_t = 0,
    MOD_BRUSH,
    MOD_SPRITE,
    MOD_STUDIO
};

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_LOWER_CHEST,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

typedef unsigned short MDLHandle_t;

struct StudioBone_t
{
    int                    sznameindex;
    inline char* const    pszName(void) const { return ((char*)this) + sznameindex; }
    int                    parent;
    int                    bonecontroller[6];
    Vector                 pos;
    Quaternion             quat;
    RadianEuler            rot;

    Vector                 posscale;
    Vector                 rotscale;

    Matrix3x4_t            poseToBone;
    Quaternion             qAlignment;
    int                    flags;
    int                    proctype;
    int                    procindex;
    mutable int            physicsbone;
    inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
    int                    surfacepropidx;
    inline char* const    pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
    inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

    int                    contents;
    int                    surfacepropLookup;
    int                    m_iPad01[7];

    StudioBone_t() {}

private:
    StudioBone_t(const StudioBone_t& other);

};


struct StudioBox_t
{
    int         bone;
    int         group;
    Vector      bbmin;
    Vector      bbmax;
    int         szhitboxnameindex;
    int32_t     m_iPad01[3];
    float       m_flRadius;
    int32_t     m_iPad02[4];

    const char* GetName()
    {
        if (!szhitboxnameindex) return nullptr;
        return (const char*)((uint8_t*)this + szhitboxnameindex);
    }
};

struct StudioHitboxSet_t
{
    int    sznameindex;
    int    numhitboxes;
    int    hitboxindex;

    const char* GetName()
    {
        if (!sznameindex) return nullptr;
        return (const char*)((uint8_t*)this + sznameindex);
    }

    StudioBox_t* GetHitbox(int i)
    {
        if (i > numhitboxes) return nullptr;
        return (StudioBox_t*)((uint8_t*)this + hitboxindex) + i;
    }
};

struct Model_t
{
    void*   fnHandle;               
    char    szName[260];         
    __int32 nLoadFlags;            
    __int32 nServerCount;         
    __int32 type;                   
    __int32 flags;                 
    Vector  vecMins;               
    Vector  vecMaxs;             
    float   radius;               
    char    pad[0x1C];             
};

class StudioHdr_t
{
public:
    __int32 id;
    __int32 version;
    long    checksum;
    char    szName[64];
    __int32 length;
    Vector  vecEyePos;
    Vector  vecIllumPos;
    Vector  vecHullMin;
    Vector  vecHullMax;
    Vector  vecBBMin;
    Vector  vecBBMax;
    __int32 flags;
    __int32 numbones;
    __int32 boneindex;
    __int32 numbonecontrollers;
    __int32 bonecontrollerindex;
    __int32 numhitboxsets;
    __int32 hitboxsetindex;
    __int32 numlocalanim;
    __int32 localanimindex;
    __int32 numlocalseq;
    __int32 localseqindex;
    __int32 activitylistversion;
    __int32 eventsindexed;
    __int32 numtextures;
    __int32 textureindex;

    StudioHitboxSet_t* GetHitboxSet(int i)
    {
        if (i > numhitboxsets) return nullptr;
        return (StudioHitboxSet_t*)((uint8_t*)this + hitboxsetindex) + i;
    }

    StudioBone_t* GetBone(int i)
    {
        if (i > numbones) return nullptr;
        return (StudioBone_t*)((uint8_t*)this + boneindex) + i;
    }

};