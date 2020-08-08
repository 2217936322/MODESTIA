#pragma once

struct CharacterSet_t
{
    char Set[256];
};

extern void CharacterSetBuild(CharacterSet_t* setBuffer, const char* pSetString);

#define IN_CHARACTERSET( SetBuffer, character )		((SetBuffer).Set[(character)])
