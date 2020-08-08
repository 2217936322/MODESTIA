#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>

#include "misc/Recv.hpp"
#include "../helpers/singleton.hpp"

class NetvarSys : public Singleton<NetvarSys>
{
    struct NetvarTable
    {
        std::string name;
        RecvProp* prop;
        uint32_t offset;
        std::vector<RecvProp*> childProps;
        std::vector<NetvarTable> childTables;
    };

public:
    void Initialize();

    void Dump();
    void Dump(std::ostream& stream);

    uint32_t GetOffset(const std::string& tableName, const std::string& propName);
    RecvProp*     GetNetvarProp(const std::string& tableName, const std::string& propName);

private:
    static NetvarTable LoadTable(RecvTable* clientClass);
    static void DumpTable(std::ostream& stream, const NetvarTable& table, uint32_t indentation);
    static uint32_t GetOffset(const NetvarTable& table, const std::string& propName);
    static RecvProp* GetNetvarProp(const NetvarTable& table, const std::string& propName);

private:
    std::vector<NetvarTable>  database;
};