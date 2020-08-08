#pragma once

#include <vector>
#include <string>

struct C_PaintKit
{
	int id;
	std::string name;
	bool operator < (const C_PaintKit& other) const
	{
		return name < other.name;
	}
};

extern std::vector<C_PaintKit> k_SkinKits;
extern std::vector<C_PaintKit> k_GloveKits;

void InitializeKits();