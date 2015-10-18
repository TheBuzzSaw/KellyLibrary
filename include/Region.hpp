#ifndef REGION_HPP_KELLY
#define REGION_HPP_KELLY

#include <cstdlib>

namespace Kelly
{
    class Region
    {
        void* _block;
        int _newPageByteCount;

        void Release() noexcept;

    public:
        Region() noexcept;
        Region(Region&& other) noexcept;
        Region(const Region&) = delete;
        ~Region() noexcept;

        Region& operator=(Region&& other) noexcept;
        Region& operator=(const Region&) = delete;

        void* Allocate(int byteCount);
        int PageCount() const noexcept;
        void DebugDump() const;

        inline int NewPageByteCount() const noexcept
        {
            return _newPageByteCount;
        }

        void SetNewPageByteCount(int newPageByteCount);
    };
}

void* operator new(size_t byteCount, Kelly::Region& region);
void* operator new[](size_t byteCount, Kelly::Region& region);

#endif
