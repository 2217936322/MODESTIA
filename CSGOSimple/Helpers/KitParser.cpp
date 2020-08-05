#include "KitParser.hpp"

#include <algorithm>

#include "../Helpers/Utils.hpp"

std::vector<C_PaintKit> k_Skins;
std::vector<C_PaintKit> k_Gloves;
std::vector<C_PaintKit> k_Stickers;

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

struct CStickerKit
{
	int id;
	int itemRarity;
	String_t name;
	String_t description;
	String_t itemName;
	String_t materialName;
	String_t imageInventory;
	int tournamentEventId;
	int tournamentTeamId;
	int tournamentPlayerId;
	bool isCustomStickerMaterial;
	float rotateEnd;
	float rotateStart;
	float scaleMin;
	float scaleMax;
	float wearMin;
	float wearMax;
	String_t imageInventory2;
	String_t imageInventoryLarge;
	std::uint32_t pad0[4];
};

auto GetExport(const char* module_name, const char* export_name) -> void*
{
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);
	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}

auto InitializeKits() -> void
{
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(GetExport("vstdlib.dll", "V_UCS2ToUTF8"));
	const auto sigAddres = Utils::PatternScan(GetModuleHandleA("client.dll"), "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
	const auto itemSystemOffset = *reinterpret_cast<std::int32_t*>(sigAddres + 1);
	const auto itemSystemFn = reinterpret_cast<CCStrike15ItemSystem * (*)()>(sigAddres + 5 + itemSystemOffset);
	const auto itemSchema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(itemSystemFn()) + sizeof(void*));

	//Dump paint kits
	{

		const auto getPaintKitDefinitionOffset = *reinterpret_cast<std::int32_t*>(sigAddres + 11 + 1);
		const auto getPaintKitDefinitionFn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(sigAddres + 11 + 5 + getPaintKitDefinitionOffset);
		const auto startElementOffset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(getPaintKitDefinitionFn) + 8 + 2);
		const auto headOffset = startElementOffset - 12;
		const auto mapHead = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(itemSchema) + headOffset);

		for (auto i = 0; i <= mapHead->lastElement; ++i)
		{
			const auto paintKit = mapHead->memory[i].value;
			if (paintKit->id == 9001)
				continue;

			const auto wideName = g_Localize->Find(paintKit->itemName.buffer + 1);
			char szName[256];

			V_UCS2ToUTF8(wideName, szName, sizeof(szName));
			if (paintKit->id < 10000)
				k_Skins.push_back({ paintKit->id, szName });
			else
				k_Gloves.push_back({ paintKit->id, szName });
		}
		std::sort(k_Skins.begin(), k_Skins.end());
		std::sort(k_Gloves.begin(), k_Gloves.end());
	}

	//Dump sticker kits
	{
		const auto sigSticker = (Utils::PatternScan(GetModuleHandleA("client.dll"), "53 8D 48 04 E8 ? ? ? ? 8B 4D 10") + 4);
		const auto getStickerKitDefinitionOffset = *reinterpret_cast<std::intptr_t*>(sigSticker + 1);
		const auto getStickerKitDefinitionFn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(sigSticker + 5 + getStickerKitDefinitionOffset);
		const auto startElementOffset = *reinterpret_cast<intptr_t*>(std::uintptr_t(getStickerKitDefinitionFn) + 8 + 2);
		const auto headOffset = startElementOffset - 12;
		const auto mapHead = reinterpret_cast<Head_t<int, CStickerKit*>*>(std::uintptr_t(itemSchema) + headOffset);

		for (auto i = 0; i <= mapHead->lastElement; ++i)
		{
			const auto stickerKit = mapHead->memory[i].value;
			char stickerNameIfValveFuckedUpTheirTranslations[64];
			auto stickerNamePtr = stickerKit->itemName.buffer + 1;

			if (strstr(stickerNamePtr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(stickerNameIfValveFuckedUpTheirTranslations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(stickerNameIfValveFuckedUpTheirTranslations, stickerNamePtr + 27);
				stickerNamePtr = stickerNameIfValveFuckedUpTheirTranslations;
			}

			const auto wideName = g_Localize->Find(stickerNamePtr);
			char name[256];
			V_UCS2ToUTF8(wideName, name, sizeof(name));

			k_Stickers.push_back({ stickerKit->id, name });
		}

		std::sort(k_Stickers.begin(), k_Stickers.end());

		k_Stickers.insert(k_Stickers.begin(), { 0, "None" });
	}
}
