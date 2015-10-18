#ifndef STATICREGION_HPP_KELLY
#define STATICREGION_HPP_KELLY

#include <cassert>

namespace Kelly
{
    template<int N> class StaticRegion
    {
        int _usedByteCount;
        char _data[N];

    public:
        StaticRegion() : _usedByteCount(0) {}
        StaticRegion(StaticRegion&&) = delete;
        StaticRegion(const StaticRegion&) = delete;
        ~StaticRegion() = default;

        StaticRegion& operator=(StaticRegion&&) = delete;
        StaticRegion& operator=(const StaticRegion&) = delete;

        void* Allocate(int byteCount)
        {
            assert(byteCount > 0);
            void* result = _data + _usedByteCount;
            _usedByteCount += byteCount;
            return result;
        }

        inline int UsedByteCount() const { return _usedByteCount; }
        inline int FreeByteCount() const { return N - _usedByteCount; }
    };
}

#endif

