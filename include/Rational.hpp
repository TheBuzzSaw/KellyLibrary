#ifndef RATIONAL_HPP_KELLY
#define RATIONAL_HPP_KELLY

#include <iostream>

namespace Kelly
{
    template<typename T> struct Rational
    {
        T a; // numerator
        T b; // denominator

        Rational& operator+=(Rational r);
        Rational& operator-=(Rational r);
        Rational& operator*=(Rational r);
        Rational& operator/=(Rational r);

        Rational& operator+=(T n);
        Rational& operator-=(T n);
        Rational& operator*=(T n);
        Rational& operator/=(T n);
    };

    template<typename T> constexpr bool IsNegative(Rational<T> r)
    {
        return (r.a < T(0)) ^ (r.b < T(0));
    }

    template<typename T> constexpr Rational<T> Normalized(Rational<T> r)
    {
        return r.b < T(0) ? Rational<T>{r.a, r.b} : Rational<T>{-r.a, -r.b};
    }

    template<typename T> constexpr T InternalCompare(
        Rational<T> r1, Rational<T> r2)
    {
        return r1.a * r2.b - r2.a * r1.b;
    }

    template<typename T> constexpr T Compare(Rational<T> r1, Rational<T> r2)
    {
        return InternalCompare(Normalized(r1), Normalized(r2));
    }

    template<typename T> constexpr Rational<T> operator-(Rational<T> r)
    {
        return {-r.a, r.b};
    }

    template<typename T> constexpr Rational<T> operator+(
        Rational<T> r1, Rational<T> r2)
    {
        return {r1.a * r2.b + r2.a * r1.b, r1.b * r2.b};
    }

    template<typename T> constexpr Rational<T> operator-(
        Rational<T> r1, Rational<T> r2)
    {
        return {r1.a * r2.b - r2.a * r1.b, r1.b * r2.b};
    }

    template<typename T> constexpr Rational<T> operator*(
        Rational<T> r1, Rational<T> r2)
    {
        return {r1.a * r2.a, r1.b * r2.b};
    }

    template<typename T> constexpr Rational<T> operator/(
        Rational<T> r1, Rational<T> r2)
    {
        return {r1.a * r2.b, r1.b * r2.a};
    }

    template<typename T> constexpr Rational<T> operator+(Rational<T> r, T n)
    {
        return {r.a + n * r.b, r.b};
    }

    template<typename T> constexpr Rational<T> operator-(Rational<T> r, T n)
    {
        return {r.a - n * r.b, r.b};
    }

    template<typename T> constexpr Rational<T> operator*(Rational<T> r, T n)
    {
        return {r.a * n, r.b};
    }

    template<typename T> constexpr Rational<T> operator/(Rational<T> r, T n)
    {
        return {r.a, r.b * n};
    }

    template<typename T> constexpr bool operator==(
        Rational<T> r1, Rational<T> r2)
    {
        return Compare(r1, r2) == T(0);
    }

    template<typename T> constexpr bool operator!=(
        Rational<T> r1, Rational<T> r2)
    {
        return Compare(r1, r2) != T(0);
    }

    template<typename T> constexpr bool operator<(
        Rational<T> r1, Rational<T> r2)
    {
        return Compare(r1, r2) < T(0);
    }

    template<typename T> constexpr bool operator>(
        Rational<T> r1, Rational<T> r2)
    {
        return Compare(r1, r2) > T(0);
    }

    template<typename T> constexpr bool operator<=(
        Rational<T> r1, Rational<T> r2)
    {
        return Compare(r1, r2) <= T(0);
    }

    template<typename T> constexpr bool operator>=(
        Rational<T> r1, Rational<T> r2)
    {
        return Compare(r1, r2) >= T(0);
    }

    template<typename T> Rational<T>& Rational<T>::operator+=(Rational<T> r)
    {
        return *this = operator+(*this, r);
    }

    template<typename T> Rational<T>& Rational<T>::operator-=(Rational<T> r)
    {
        return *this = operator-(*this, r);
    }

    template<typename T> Rational<T>& Rational<T>::operator*=(Rational<T> r)
    {
        return *this = operator*(*this, r);
    }

    template<typename T> Rational<T>& Rational<T>::operator/=(Rational<T> r)
    {
        return *this = operator/(*this, r);
    }

    template<typename T> Rational<T>& Rational<T>::operator+=(T n)
    {
        return *this = operator+(*this, n);
    }

    template<typename T> Rational<T>& Rational<T>::operator-=(T n)
    {
        return *this = operator-(*this, n);
    }

    template<typename T> Rational<T>& Rational<T>::operator*=(T n)
    {
        return *this = operator*(*this, n);
    }

    template<typename T> Rational<T>& Rational<T>::operator/=(T n)
    {
        return *this = operator/(*this, n);
    }

    template<typename T> std::ostream& operator<<(
        std::ostream& stream, Rational<T> r)
    {
        return stream << r.a << '/' << r.b;
    }
}

#endif
