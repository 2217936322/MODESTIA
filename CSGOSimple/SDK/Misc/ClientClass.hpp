#pragma once

#include "Recv.hpp"

class C_BaseEntity;
class ClientClass;
class IClientNetworkable;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
    //CreateClientClassFn      m_pCreateFn;
    std::add_pointer_t<C_BaseEntity* __cdecl(int, int)> createFunction;
    CreateEventFn            m_pCreateEventFn;
    char*                    m_pNetworkName;
    RecvTable*               m_pRecvTable;
    ClientClass*             m_pNext;
    ClassID                  m_ClassID;
};