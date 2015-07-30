#include "../include/CString.hpp"
#include <cstring>

namespace Kelly
{
    ptrdiff_t CString::Length() const
    {
        return value ? strlen(value) : 0;
    }

    int Compare(const CString& a, const CString& b)
    {
        bool hasContent = a.HasContent();
        int result = hasContent - b.HasContent();
        if (!result && hasContent) result = strcmp(a.value, b.value);
        return result;
    }

    bool operator==(const CString& a, const CString& b)
    {
        return !Compare(a, b);
    }

    bool operator!=(const CString& a, const CString& b)
    {
        return Compare(a, b);
    }

    bool operator<(const CString& a, const CString& b)
    {
        return Compare(a, b) < 0;
    }

    bool operator>(const CString& a, const CString& b)
    {
        return Compare(a, b) > 0;
    }

    bool operator<=(const CString& a, const CString& b)
    {
        return Compare(a, b) <= 0;
    }

    bool operator>=(const CString& a, const CString& b)
    {
        return Compare(a, b) >= 0;
    }

}
