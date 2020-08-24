#pragma once

#include <cstdint>

namespace FNV 
{
    constexpr uint32_t offsetBasis = 0x811c9dc5;
    constexpr uint32_t prime = 0x1000193;

    constexpr uint32_t Hash(const char* str, const uint32_t value = offsetBasis)
    {
        return *str ? Hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
    }

    constexpr uint32_t HashRuntime(const char* str)
    {
        auto value = offsetBasis;

        while (*str)
        {
            value ^= *str++;
            value *= prime;
        }
        return value;
    }
}