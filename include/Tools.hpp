#ifndef TOOLS_HPP_KELLY
#define TOOLS_HPP_KELLY

#include <algorithm>

namespace Kelly
{
    constexpr bool HasContent(const char* text) { return text && *text; }

    template<typename T> const T& Get(const void* address)
    {
        return *reinterpret_cast<const T*>(address);
    }

    template<typename T> T& EndianSwap(T& value)
    {
        constexpr auto HalfSize = sizeof(T) / 2;
        constexpr auto LastIndex = sizeof(T) - 1;

        auto raw = (uint8_t*)&value;
        for (size_t i = 0; i < HalfSize; ++i)
            std::swap(raw[i], raw[LastIndex - i]);

        return value;
    }

    template<typename T> T EndianSwapped(T value)
    {
        return EndianSwap(value);
    }
}

#endif
