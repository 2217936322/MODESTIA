#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>

#include "Misc/Recv.hpp"
#include "../Helpers/Singleton.hpp"

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

    uint32_t GetOffset(const std::string& tableName, const std::string& propName);
    RecvProp* GetNetvarProp(const std::string& tableName, const std::string& propName);

private:
    static NetvarTable LoadTable(RecvTable* clientClass);
    static uint32_t GetOffset(const NetvarTable& table, const std::string& propName);
    static RecvProp* GetNetvarProp(const NetvarTable& table, const std::string& propName);

private:
    std::vector<NetvarTable> database;
};