#pragma once

#include "IHandleEntity.hpp"

#define NUM_ENT_ENTRY_BITS         (11 + 2)
#define NUM_ENT_ENTRIES            (1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX       0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS        16
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK             (( 1 << NUM_SERIAL_NUM_BITS) - 1)

class IHandleEntity;

class CBaseHandle
{
public:
    CBaseHandle();
    CBaseHandle(const CBaseHandle& other);
    CBaseHandle(unsigned long value);
    CBaseHandle(int entry, int serialNumber);

    void Init(int entry, int serialNumber);
    void Term();

    bool IsValid() const;

    int GetEntryIndex() const;
    int GetSerialNumber() const;

    int ToInt() const;
    bool operator !=(const CBaseHandle& other) const;
    bool operator ==(const CBaseHandle& other) const;
    bool operator ==(const IHandleEntity* pEnt) const;
    bool operator !=(const IHandleEntity* pEnt) const;
    bool operator <(const CBaseHandle& other) const;
    bool operator <(const IHandleEntity* pEnt) const;
    const CBaseHandle& operator=(const IHandleEntity* entity);
    const CBaseHandle& Set(const IHandleEntity* entity);

    IHandleEntity* Get() const;

protected:
    unsigned long	m_Index;
};

inline CBaseHandle::CBaseHandle()
{
    m_Index = INVALID_EHANDLE_INDEX;
}

inline CBaseHandle::CBaseHandle(const CBaseHandle& other)
{
    m_Index = other.m_Index;
}

inline CBaseHandle::CBaseHandle(unsigned long value)
{
    m_Index = value;
}

inline CBaseHandle::CBaseHandle(int entry, int serialNumber)
{
    Init(entry, serialNumber);
}

inline void CBaseHandle::Init(int entry, int serialNumber)
{
    m_Index = entry | (serialNumber << NUM_ENT_ENTRY_BITS);
}

inline void CBaseHandle::Term()
{
    m_Index = INVALID_EHANDLE_INDEX;
}

inline bool CBaseHandle::IsValid() const
{
    return m_Index != INVALID_EHANDLE_INDEX;
}

inline int CBaseHandle::GetEntryIndex() const
{
    return m_Index & ENT_ENTRY_MASK;
}

inline int CBaseHandle::GetSerialNumber() const
{
    return m_Index >> NUM_ENT_ENTRY_BITS;
}

inline int CBaseHandle::ToInt() const
{
    return (int)m_Index;
}

inline bool CBaseHandle::operator !=(const CBaseHandle& other) const
{
    return m_Index != other.m_Index;
}

inline bool CBaseHandle::operator ==(const CBaseHandle& other) const
{
    return m_Index == other.m_Index;
}

inline bool CBaseHandle::operator ==(const IHandleEntity* pEnt) const
{
    return Get() == pEnt;
}

inline bool CBaseHandle::operator !=(const IHandleEntity* pEnt) const
{
    return Get() != pEnt;
}

inline bool CBaseHandle::operator <(const CBaseHandle& other) const
{
    return m_Index < other.m_Index;
}

inline bool CBaseHandle::operator <(const IHandleEntity* entity) const
{
    unsigned long otherIndex = (entity) ? entity->GetRefEHandle().m_Index : INVALID_EHANDLE_INDEX;
    return m_Index < otherIndex;
}

inline const CBaseHandle& CBaseHandle::operator=(const IHandleEntity* entity)
{
    return Set(entity);
}

inline const CBaseHandle& CBaseHandle::Set(const IHandleEntity* entity)
{
    if (entity)
    {
        *this = entity->GetRefEHandle();
    }
    else
    {
        m_Index = INVALID_EHANDLE_INDEX;
    }

    return *this;
}