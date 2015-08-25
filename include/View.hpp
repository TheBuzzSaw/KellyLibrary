#ifndef VIEW_HPP_KELLY
#define VIEW_HPP_KELLY

#include "Math.hpp"
#include <cstdint>
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

    template<typename T> void Copy(View<T> destination, View<const T> source)
    {
        memcpy(
            destination.data,
            source.data,
            sizeof(T) * Min(destination.n, source.n));
    }

    template<typename T> void Zero(View<T> view)
    {
        memset(view, 0, sizeof(T) * view.n);
    }
}

#endif
