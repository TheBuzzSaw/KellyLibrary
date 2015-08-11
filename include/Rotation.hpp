#ifndef ROTATION_HPP
#define ROTATION_HPP

#include "Math.hpp"
#include <cmath>

namespace Kelly
{
    template<typename T> class Rotation
    {
        T _radians;

    public:
        constexpr Rotation() : _radians(0) {}
        explicit constexpr Rotation(T radians) : _radians(radians) {}
        constexpr Rotation(const Rotation&) = default;
        ~Rotation() = default;

        Rotation& operator=(const Rotation&) = default;

        Rotation& operator+=(const Rotation& other)
        {
            _radians += other._radians;
            if (_radians >= Tau<T>()) _radians -= Tau<T>();
            return *this;
        }

        Rotation& operator-=(const Rotation& other)
        {
            _radians -= other._radians;
            if (_radians < T(0)) _radians += Tau<T>();
            return *this;
        }

        constexpr T ToRadians() const { return _radians; }
        constexpr T ToDegrees() const { return Kelly::ToDegrees<T>(_radians); }
    };

    template<typename T> inline Rotation<T> operator+(
        const Rotation<T>& a,
        const Rotation<T>& b)
    {
        return Rotation<T>(a) += b;
    }

    template<typename T> inline Rotation<T> operator-(
        const Rotation<T>& a,
        const Rotation<T>& b)
    {
        return Rotation<T>(a) -= b;
    }

    template<typename T> constexpr Rotation<T> operator-(const Rotation<T>& a)
    {
        return Rotation<T>(Tau<T>() - a.ToRadians());
    }

    template<typename T> inline Rotation<T> Opposite(const Rotation<T>& a)
    {
        return Rotation<T>(a) += Rotation<T>(Pi<T>());
    }

    template<typename T> Rotation<T> Radians(T radians)
    {
        radians = std::fmod(radians, Tau<T>());
        return Rotation<T>(radians < T(0) ? radians + Tau<T>() : radians);
    }

    template<typename T> inline Rotation<T> Degrees(T degrees)
    {
        return Radians(degrees * Pi<T>() / T(180));
    }

    using Rotation32 = Rotation<float>;
    using Rotation64 = Rotation<double>;
}

#endif

