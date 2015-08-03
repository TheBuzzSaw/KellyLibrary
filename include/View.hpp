#ifndef VIEW_HPP_KELLY
#define VIEW_HPP_KELLY

#include "Math.hpp"
#include <cstring>
#include <cstddef>

namespace Kelly
{
    template<typename T> struct View
    {
        T* data;
        ptrdiff_t n;

        constexpr operator View<const T>() const { return { data, n }; }
        constexpr T* begin() const { return data; }
        constexpr T* end() const { return data + n; }
    };

    template<typename T> constexpr View<uint8_t> ToByteView(
        const View<T>& view)
    {
        return { (uint8_t*)view.data, view.n * sizeof(T) };
    }

    template<typename T> constexpr View<const uint8_t> ToReadOnlyByteView(
        const View<T>& view)
    {
        return { (const uint8_t*)view.data, view.n * sizeof(T) };
    }

    template<typename T> inline void Copy(
        const View<T>& destination,
        const View<const T>& source)
    {
        memcpy(
            destination.data,
            source.data,
            sizeof(T) * Min(destination.n, source.n));
    }

    template<typename T> inline void Zero(const View<T>& view)
    {
        memset(view, 0, sizeof(T) * view.n);
    }
}

#endif
