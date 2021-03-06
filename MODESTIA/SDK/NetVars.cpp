#include "Netvars.hpp"

#include <fstream>
#include <utility>
#include <map>

#include "SDK.hpp"

void NetvarSys::Initialize()
{
    database.clear();

    for (auto clientclass = g_CHLClient->GetAllClasses(); clientclass != nullptr; clientclass = clientclass->m_pNext)
    {
        if (clientclass->m_pRecvTable)
        {
            database.emplace_back(LoadTable(clientclass->m_pRecvTable));
        }
    }
}

NetvarSys::NetvarTable NetvarSys::LoadTable(RecvTable* recvTable)
{
    auto table = NetvarTable{};

    table.offset = 0;
    table.name = recvTable->m_pNetTableName;

    for (auto i = 0; i < recvTable->m_nPropsCount; ++i)
    {
        auto prop = &recvTable->m_pProps[i];

        if (!prop || isdigit(prop->m_pVarName[0]))
            continue;

        if (strcmp("baseclass", prop->m_pVarName) == 0)
            continue;

        if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
        {
            table.childTables.emplace_back(LoadTable(prop->m_pDataTable));
            table.childTables.back().offset = prop->m_Offset;
            table.childTables.back().prop = prop;
        }
        else
        {
            table.childProps.emplace_back(prop);
        }
    }
    return table;
}

uint32_t NetvarSys::GetOffset(const std::string& tableName, const std::string& propName)
{
    auto result = 0u;
    for (const auto& table : database)
    {
        if (table.name == tableName)
        {
            result = GetOffset(table, propName);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

uint32_t NetvarSys::GetOffset(const NetvarSys::NetvarTable& table, const std::string& propName)
{
    for (const auto& prop : table.childProps)
    {
        if (strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0)
        {
            return table.offset + prop->m_Offset;
        }
    }

    for (const auto& child : table.childTables)
    {
        auto propOffset = GetOffset(child, propName);
        if (propOffset != 0)
            return table.offset + propOffset;
    }

    for (const auto& child : table.childTables)
    {
        if (strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0)
        {
            return table.offset + child.offset;
        }
    }

    return 0;
}

RecvProp* NetvarSys::GetNetvarProp(const std::string& tableName, const std::string& propName)
{
    RecvProp* result = nullptr;

    for (const auto& table : database) 
    {
        if (table.name == tableName) 
        {
            result = GetNetvarProp(table, propName);
        }
    }

    return result;
}

RecvProp* NetvarSys::GetNetvarProp(const NetvarSys::NetvarTable& table, const std::string& propName)
{
    for(const auto& prop : table.childProps) 
    {
        if(strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0) 
        {
            return prop;
        }
    }

    for(const auto& child : table.childTables) 
    {
        auto prop = GetNetvarProp(child, propName);
        if(prop != 0)
            return prop;
    }

    for(const auto& child : table.childTables) 
    {
        if(strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0)
        {
            return child.prop;
        }
    }

    return nullptr;
}