#ifndef MATH_HPP_KELLY
#define MATH_HPP_KELLY

namespace Kelly
{
    template<typename T> constexpr T Pi()
    {
        return 3.1415926535897932384626433832795028841971693993751058209749445;
    }

    template<typename T> constexpr T Tau()
    {
        return Pi<T>() * T(2);
    }

    template<typename T> constexpr T ToDegrees(T radians)
    {
        return radians * T(180) / Pi<T>();
    }

    template<typename T> constexpr T ToRadians(T degrees)
    {
        return degrees * Pi<T>() / T(180);
    }

    template<typename T> constexpr T Min(T a, T b)
    {
        return b < a ? b : a;
    }

    template<typename T> constexpr T Max(T a, T b)
    {
        return b < a ? a : b;
    }

    template<typename T> constexpr T Bound(T value, T low, T high)
    {
        return Min(Max(value, low), high);
    }

    template<typename T> constexpr bool InRange(T value, T low, T high)
    {
        return low <= value && value <= high;
    }
}

#endif
