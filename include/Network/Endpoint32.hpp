#ifndef ENDPOINT32_HPP_KELLY
#define ENDPOINT32_HPP_KELLY

#include "Address32.hpp"

namespace Kelly
{
    struct Endpoint32
    {
        Address32 address;
        uint16_t port;
    };

    constexpr bool operator==(const Endpoint32& a, const Endpoint32& b)
    {
        return a.address == b.address && a.port == b.port;
    }

    constexpr bool operator!=(const Endpoint32& a, const Endpoint32& b)
    {
        return a.address != b.address || a.port != b.port;
    }

    constexpr Endpoint32 NullEndpoint32 = {{{0,0,0,0}},0};

    template<typename CharT, typename TraitsT>
    std::basic_ostream<CharT, TraitsT>& operator<<(
        std::basic_ostream<CharT, TraitsT>& stream,
        const Endpoint32& endpoint)
    {
        return stream
            << endpoint.address
            << (CharT)':'
            << endpoint.port;
    }
}

#endif
