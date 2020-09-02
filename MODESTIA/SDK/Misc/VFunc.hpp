#pragma once

template<typename FuncType>
__forceinline static FuncType CallVirtualFunction(void* class_, int index)
{
    int* virtualTable = *(int**)class_;
    int addres = virtualTable[index];
    return (FuncType)(addres);
}
