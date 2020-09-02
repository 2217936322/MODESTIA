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

extern std::vector<C_PaintKit> SkinKits;
extern std::vector<C_PaintKit> GloveKits;

void InitializeKits();