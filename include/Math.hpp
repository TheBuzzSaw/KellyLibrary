#ifndef MATH_HPP_KELLY
#define MATH_HPP_KELLY

namespace Kelly
{
    template<typename T> constexpr T Min(const T& a, const T& b)
    {
        return b < a ? b : a;
    }

    template<typename T> constexpr T Max(const T& a, const T& b)
    {
        return a < b ? b : a;
    }

    template<typename T>
    constexpr T Bound(const T& value, const T& low, const T& high)
    {
        return Min(Max(value, low), high);
    }

    template<typename T>
    constexpr bool InRange(const T& value, const T& low, const T& high)
    {
        return low <= value && value <= high;
    }
}

#endif
