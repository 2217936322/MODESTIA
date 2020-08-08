#include <string.h>
#include "characterset.hpp"

void CharacterSetBuild(CharacterSet_t* setBuffer, const char* setString)
{
    int i = 0;

    if (!setBuffer || !setString)
        return;

    memset(setBuffer->Set, 0, sizeof(setBuffer->Set));

    while (setString[i])
    {
        setBuffer->Set[setString[i]] = 1;
        i++;
    }
}