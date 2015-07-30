#ifndef ROTATION_HPP
#define ROTATION_HPP

#include "Math.hpp"

namespace Kelly
{
    template<typename T> class Rotation
    {
        T _radians;

        explicit constexpr Rotation(T radians) : _radians(radians) {}

    public:
        constexpr Rotation() : _radians(0) {}
        constexpr Rotation(const Rotation&) = default;
        ~Rotation() = default;

        Rotation& operator=(const Rotation&) = default;

        constexpr T ToRadians() const { return _radians; }
        constexpr T ToDegrees() const { return _radians * T(180) / Pi<T>(); }

        static constexpr Rotation FromRadians(T radians)
        {
            return Rotation(radians);
        }

        static constexpr Rotation FromDegrees(T degrees)
        {
            return Rotation(degrees * Pi<T>() / T(180));
        }
    };

    using Rotation32 = Rotation<float>;
    using Rotation64 = Rotation<double>;
}

#endif

