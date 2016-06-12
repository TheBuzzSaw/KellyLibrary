#ifndef CSTRING_HPP
#define CSTRING_HPP

#include <string>
#include <iostream>
#include <cstddef>

namespace Kelly
{
    struct CString
    {
        const char* value;

        constexpr CString() : value(nullptr) {}
        constexpr CString(const char* cc) : value(cc) {}
        inline CString(const std::string& s) : value(s.data()) {}
        constexpr CString(const CString&) = default;
        ~CString() = default;

        CString& operator=(const CString&) = default;
        CString& operator+=(ptrdiff_t n) { value += n; return *this; }
        CString& operator-=(ptrdiff_t n) { value -= n; return *this; }

        constexpr bool HasContent() const { return value && *value; }
        constexpr bool IsNull() const { return value == nullptr; }
        constexpr char operator[](ptrdiff_t index) const
        {
            return value[index];
        }

        ptrdiff_t Length() const;
    };

    constexpr CString operator+(CString cs, ptrdiff_t n)
    {
        return cs.value + n;
    }

    constexpr CString operator-(CString cs, ptrdiff_t n)
    {
        return cs.value - n;
    }

    int Compare(CString a, CString b);
    bool operator==(CString a, CString b);
    bool operator!=(CString a, CString b);
    bool operator<(CString a, CString b);
    bool operator>(CString a, CString b);
    bool operator<=(CString a, CString b);
    bool operator>=(CString a, CString b);

    inline std::ostream& operator<<(
        std::ostream& stream,
        CString cstring)
    {
        if (cstring.value) stream << cstring.value;
        return stream;
    }
}

#endif
