#include "netvars.hpp"

#include <fstream>
#include <utility>
#include <map>

#include "sdk.hpp"

void NetvarSys::Initialize()
{
    database.clear();

    for(auto clientclass = g_CHLClient->GetAllClasses();
        clientclass != nullptr;
        clientclass = clientclass->m_pNext) {
        if(clientclass->m_pRecvTable) {
            database.emplace_back(LoadTable(clientclass->m_pRecvTable));
        }
    }
}

NetvarSys::netvar_table NetvarSys::LoadTable(RecvTable* recvTable)
{
    auto table = netvar_table{};

    table.offset = 0;
    table.name = recvTable->m_pNetTableName;

    for(auto i = 0; i < recvTable->m_nPropsCount; ++i) {
        auto prop = &recvTable->m_pProps[i];

        if(!prop || isdigit(prop->m_pVarName[0]))
            continue;
        if(strcmp("baseclass", prop->m_pVarName) == 0)
            continue;

        if(prop->m_RecvType == DPT_DataTable && prop->m_pDataTable) {
            table.child_tables.emplace_back(LoadTable(prop->m_pDataTable));
            table.child_tables.back().offset = prop->m_Offset;
            table.child_tables.back().prop = prop;
        } else {
            table.child_props.emplace_back(prop);
        }
    }
    return table;
}

void NetvarSys::Dump()
{
    auto outfile = std::ofstream("netvar_dump.txt");

    Dump(outfile);
}

void NetvarSys::Dump(std::ostream& stream)
{
    for(const auto& table : database) {
        if(table.child_props.empty() && table.child_tables.empty())
            continue;
        stream << table.name << '\n';
        DumpTable(stream, table, 1);
        stream << '\n';
    }

    stream << std::endl;
}

void NetvarSys::DumpTable(std::ostream& stream, const netvar_table& table, uint32_t indentation)
{
    char line_buffer[1024];

    for(const auto& prop : table.child_props) {
        sprintf_s(line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -(50 - (int)indentation * 4), prop->m_pVarName, table.offset + prop->m_Offset);
        stream << line_buffer << '\n';
    }
    for(const auto& child : table.child_tables) {
        sprintf_s(line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -(50 - (int)indentation * 4), child.prop->m_pVarName, table.offset + child.offset);
        stream << line_buffer << '\n';
        DumpTable(stream, child, indentation + 1);
    }
}

uint32_t NetvarSys::GetOffset(const std::string& tableName, const std::string& propName)
{
    auto result = 0u;
    for(const auto& table : database) {
        if(table.name == tableName) {
            result = GetOffset(table, propName);
            if(result != 0)
                return result;
        }
    }
    return 0;
}

uint32_t NetvarSys::GetOffset(const NetvarSys::netvar_table& table, const std::string& propName)
{
    for(const auto& prop : table.child_props) {
        if(strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0) {
            return table.offset + prop->m_Offset;
        }
    }
    for(const auto& child : table.child_tables) {
        auto prop_offset = GetOffset(child, propName);
        if(prop_offset != 0)
            return table.offset + prop_offset;
    }
    for(const auto& child : table.child_tables) {
        if(strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0) {
            return table.offset + child.offset;
        }
    }
    return 0;
}

RecvProp* NetvarSys::GetNetvarProp(const std::string& tableName, const std::string& propName)
{
    RecvProp* result = nullptr;
    for(const auto& table : database) {
        if(table.name == tableName) {
            result = GetNetvarProp(table, propName);
        }
    }
    return result;
}

RecvProp* NetvarSys::GetNetvarProp(const NetvarSys::netvar_table& table, const std::string& propName)
{
    for(const auto& prop : table.child_props) {
        if(strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0) {
            return prop;
        }
    }
    for(const auto& child : table.child_tables) {
        auto prop = GetNetvarProp(child, propName);
        if(prop != 0)
            return prop;
    }
    for(const auto& child : table.child_tables) {
        if(strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0) {
            return child.prop;
        }
    }
    return nullptr;
}


namespace NetvarManager {
    using netvar_key_value_map = std::unordered_map< uint32_t, uintptr_t >;
    using netvar_table_map = std::unordered_map< uint32_t, netvar_key_value_map >;
    void initProps(netvar_table_map& table_map);

    uintptr_t get_net_var(const uint32_t table,
        const uint32_t prop) {
        static netvar_table_map map = {};
        if (map.empty()) {
            initProps(map);
        }

        if (map.find(table) == map.end()) {
            return 0;
        }

        netvar_key_value_map& table_map = map.at(table);
        if (table_map.find(prop) == table_map.end()) {
            return 0;
        }

        return table_map.at(prop);
    }

    void add_props_for_table(netvar_table_map& table_map, const uint32_t table_name_hash, const std::string& table_name, RecvTable* table, const bool dump_vars, std::map< std::string, std::map< uintptr_t, std::string > >& var_dump, const size_t child_offset = 0) {
        for (auto i = 0; i < table->m_nPropsCount; ++i) {
            auto& prop = table->m_pProps[i];

            if (prop.m_pDataTable && prop.m_nElements > 0) {
                if (std::string(prop.m_pParentArrayPropName).substr(0, 1) == std::string("0"))
                    continue;

                add_props_for_table(table_map, table_name_hash, table_name, prop.m_pDataTable, dump_vars, var_dump, prop.m_Offset + child_offset);
            }

            auto name = std::string(prop.m_pParentArrayPropName);

            if (name.substr(0, 1) != "m" /*&& name.substr( 0, 1 ) != "b"*/)
                continue;

            const auto name_hash = fnv::hash(prop.m_pParentArrayPropName);
            const auto offset = uintptr_t(prop.m_Offset) + child_offset;

            table_map[table_name_hash][name_hash] = offset;

            if (dump_vars)
            {
                var_dump[table_name][offset] = prop.m_pParentArrayPropName;
            }
        }
    }

    void initProps(netvar_table_map& table_map) {
        const auto dump_vars = true;  //true if netvar dump

        std::map< std::string, std::map< uintptr_t, std::string > > var_dump;
        for (auto client_class = g_CHLClient->GetClientClasses();
            client_class;
            client_class = client_class->m_pNext)
        {
            const auto table = reinterpret_cast<RecvTable*>(client_class->m_pRecvTable);
            const auto table_name = table->m_pNetTableName;
            const auto table_name_hash = fnv::hash(table_name);

            if (table == nullptr)
                continue;

            add_props_for_table(table_map, table_name_hash, table_name, table, dump_vars, var_dump);
        }
    }
}