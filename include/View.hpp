#ifndef VIEW_HPP_KELLY
#define VIEW_HPP_KELLY

#include <cstddef>

namespace Kelly
{
    template<typename T> struct View
    {
        T* data;
        ptrdiff_t count;

        constexpr operator View<const T>() const { return { data, count }; }
    };

    template<typename T> constexpr T* begin(const View<T>& view)
    {
        return view.data;
    }

    template<typename T>  constexpr T* end(const View<T>& view)
    {
        return view.data + view.count;
    }
}

#endif
