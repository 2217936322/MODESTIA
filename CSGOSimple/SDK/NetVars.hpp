#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>

#include "misc/Recv.hpp"
#include "../helpers/singleton.hpp"

class NetvarSys
    : public Singleton<NetvarSys>
{
    struct netvar_table
    {
        std::string               name;
        RecvProp*                 prop;
        uint32_t                  offset;
        std::vector<RecvProp*>    child_props;
        std::vector<netvar_table> child_tables;
    };
public:
    void Initialize();

    void Dump();
    void Dump(std::ostream& stream);



    uint32_t      GetOffset(const std::string& tableName, const std::string& propName);
    RecvProp*     GetNetvarProp(const std::string& tableName, const std::string& propName);


private:
    static netvar_table  LoadTable(RecvTable* clientClass);
    static void          DumpTable(std::ostream& stream, const netvar_table& table, uint32_t indentation);
    static uint32_t      GetOffset(const netvar_table& table, const std::string& propName);
    static RecvProp*     GetNetvarProp(const netvar_table& table, const std::string& propName);

private:
    std::vector<netvar_table>  database;
};

namespace NetvarManager {
#pragma warning( push )
#pragma warning( disable: 4307 )
	namespace fnv_1a {
		template< typename S >
		struct fnv_internal;
		template< typename S >
		struct fnv1a;

		template< >
		struct fnv_internal< uint32_t > {
			constexpr static uint32_t default_offset_basis = 0x811C9DC5;
			constexpr static uint32_t prime = 0x01000193;
		};

		template< >
		struct fnv1a< uint32_t > : public fnv_internal< uint32_t > {
			constexpr static uint32_t hash(char const* const aString, const uint32_t val = default_offset_basis) {
				return (aString[0] == '\0')
					? val
					: hash(&aString[1], (val ^ uint32_t(aString[0])) * prime);
			}

			constexpr static uint32_t hash(wchar_t const* const aString, const uint32_t val = default_offset_basis) {
				return (aString[0] == L'\0')
					? val
					: hash(&aString[1], (val ^ uint32_t(aString[0])) * prime);
			}
		};
	}

	using fnv = fnv_1a::fnv1a< uint32_t >;

#pragma warning( pop )
}

namespace NetvarManager {
	uintptr_t get_net_var(uint32_t table, uint32_t prop);
}