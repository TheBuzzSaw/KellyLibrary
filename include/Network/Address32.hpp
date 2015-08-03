#ifndef ADDRESS32_HPP_KELLY
#define ADDRESS32_HPP_KELLY

#include <iostream>
#include <cstdint>

namespace Kelly
{
    union Address32
    {
        uint8_t bytes[4];
        uint32_t networkValue;
    };

    constexpr bool operator==(const Address32& a, const Address32& b)
    {
        return a.networkValue == b.networkValue;
    }

    constexpr bool operator!=(const Address32& a, const Address32& b)
    {
        return a.networkValue != b.networkValue;
    }

    constexpr Address32 NullAddress32 = { 0, 0, 0, 0 };
    constexpr Address32 LocalHost32 = { 127, 0, 0, 1 };

    template<typename CharT, typename TraitsT>
    std::basic_ostream<CharT, TraitsT>& operator<<(
        std::basic_ostream<CharT, TraitsT>& stream,
        const Address32& address)
    {
        constexpr CharT Dot = '.';

        return stream
            << (int)address.bytes[0] << Dot
            << (int)address.bytes[1] << Dot
            << (int)address.bytes[2] << Dot
            << (int)address.bytes[3];
    }
}

#endif

