#pragma once

#include "Recv.hpp"

class CBaseEntity;
class ClientClass;
class IClientNetworkable;

typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
    std::add_pointer_t<CBaseEntity* __cdecl(int, int)> createFunction;
    CreateEventFn            m_pCreateEventFn;
    char*                    m_pNetworkName;
    RecvTable*               m_pRecvTable;
    ClientClass*             m_pNext;
    ClassID                  m_ClassID;
};