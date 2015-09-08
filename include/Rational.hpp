#ifndef RATIONAL_HPP_KELLY
#define RATIONAL_HPP_KELLY

#include <iostream>

namespace Kelly
{
    template<typename T> struct Rational
    {
        T a; // numerator
        T b; // denominator
    };

    template<typename T> constexpr bool IsNegative(Rational<T> r)
    {
        return (r.a < T(0)) ^ (r.b < T(0));
    }

    template<typename T> constexpr Rational<T> operator-(Rational<T> r)
    {
        return {-r.a, r.b};
    }

    template<typename T> constexpr Rational<T> operator*(Rational<T> r, T n)
    {
        return {r.a * n, r.b};
    }

    template<typename T> constexpr Rational<T> operator/(Rational<T> r, T n)
    {
        return {r.a, r.b * n};
    }

    template<typename T> constexpr Rational<T> operator+(
        Rational<T> a, Rational<T> b)
    {
        return {a.a * b.b + b.a * a.b, a.b * b.b};
    }

    template<typename T> constexpr Rational<T> operator-(
        Rational<T> a, Rational<T> b)
    {
        return {a.a * b.b - b.a * a.b, a.b * b.b};
    }

    template<typename T> constexpr Rational<T> operator*(
        Rational<T> a, Rational<T> b)
    {
        return {a.a * b.a, a.b * b.b};
    }

    template<typename T> constexpr Rational<T> operator/(
        Rational<T> a, Rational<T> b)
    {
        return {a.a * b.b, a.b * b.a};
    }

    template<typename T> std::ostream& operator<<(
        std::ostream& stream, Rational<T> rational)
    {
        return stream << rational.a << '/' << rational.b;
    }
}

#endif
