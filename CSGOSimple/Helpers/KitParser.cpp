#include "KitParser.hpp"

#include <algorithm>

#include "../Helpers/Utils.hpp"

std::vector<C_PaintKit> k_SkinKits;
std::vector<C_PaintKit> k_GloveKits;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct Node_t
{
	int previousID;
	int nextID;
	void* _unknownPtr;
	int _unknown;
	Key key;
	Value value;
};

template <typename Key, typename Value>
struct Head_t
{
	Node_t<Key, Value>* memory;
	int allocationCount;
	int growSize;
	int startElement;
	int nextAvailable;
	int _unknown;
	int lastElement;
};

struct String_t
{
	char* buffer;
	int capacity;
	int growSize;
	int length;
};

struct CPaintKit
{
	int id;
	String_t name;
	String_t description;
	String_t itemName;
	String_t materialName;
	String_t imageInventory;
	char pad0x0054[0x8C];
};

auto GetExport(const char* module_name, const char* export_name) -> void*
{
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);
	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}

void InitializeKits()
{
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(GetExport("vstdlib.dll", "V_UCS2ToUTF8"));
	const auto patternAddres = Utils::PatternScan(GetModuleHandleA("client.dll"), "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
	const auto itemSystemOffset = *reinterpret_cast<std::int32_t*>(patternAddres + 1);
	const auto itemSystemFn = reinterpret_cast<CCStrike15ItemSystem*(*)()>(patternAddres + 5 + itemSystemOffset);
	const auto itemSchema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(itemSystemFn()) + sizeof(void*));

	//Dump paint kits
	{

		const auto getPaintKitDefinitionOffset = *reinterpret_cast<std::int32_t*>(patternAddres + 11 + 1);
		const auto getPaintKitDefinitionFn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(patternAddres + 11 + 5 + getPaintKitDefinitionOffset);
		const auto startElementOffset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(getPaintKitDefinitionFn) + 8 + 2);
		const auto headOffset = startElementOffset - 12;
		const auto mapHead = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(itemSchema) + headOffset);

		for (auto i = 0; i <= mapHead->lastElement; ++i)
		{
			const auto paintKit = mapHead->memory[i].value;
			if (paintKit->id == 9001)
				continue;

			const auto wideName = g_Localize->Find(paintKit->itemName.buffer + 1);
			char name[256];

			V_UCS2ToUTF8(wideName, name, sizeof(name));

			if (paintKit->id < 10000)
				k_SkinKits.push_back({ paintKit->id, name });
			else
				k_GloveKits.push_back({ paintKit->id, name });
		}
		std::sort(k_SkinKits.begin(), k_SkinKits.end());
		std::sort(k_GloveKits.begin(), k_GloveKits.end());
	}
}
