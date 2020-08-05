#pragma once

#include <vector>
#include <string>

struct C_PaintKit
{
	int id;
	std::string name;
	auto operator < (const C_PaintKit& other) const -> bool
	{
		return name < other.name;
	}
};

extern std::vector<C_PaintKit> k_Skins;
extern std::vector<C_PaintKit> k_Gloves;
extern std::vector<C_PaintKit> k_Stickers;
extern auto InitializeKits() -> void;