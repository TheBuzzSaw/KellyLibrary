#include "../include/CString.hpp"
#include <cstring>

namespace Kelly
{
    ptrdiff_t CString::Length() const
    {
        return value ? strlen(value) : 0;
    }

    int Compare(CString a, CString b)
    {
        bool hasContent = a.HasContent();
        int result = hasContent - b.HasContent();
        if (!result && hasContent) result = strcmp(a.value, b.value);
        return result;
    }

    bool operator==(CString a, CString b)
    {
        return !Compare(a, b);
    }

    bool operator!=(CString a, CString b)
    {
        return Compare(a, b);
    }

    bool operator<(CString a, CString b)
    {
        return Compare(a, b) < 0;
    }

    bool operator>(CString a, CString b)
    {
        return Compare(a, b) > 0;
    }

    bool operator<=(CString a, CString b)
    {
        return Compare(a, b) <= 0;
    }

    bool operator>=(CString a, CString b)
    {
        return Compare(a, b) >= 0;
    }

}
