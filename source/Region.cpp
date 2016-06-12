#include "../include/Region.hpp"
#include "../include/Math.hpp"
#include <iostream>
#include <cassert>

namespace Kelly
{
    static constexpr int DefaultNewPageByteCount = 1 << 20;

    struct PageHeader
    {
        PageHeader* next;
        char* freeBlock;
        int pageByteCount;
        int freeByteCount;
        int allocationCount;
    };

    void Region::Release() noexcept
    {
        auto header = (PageHeader*)_block;

        while (header)
        {
            auto nextHeader = header->next;
            free(header);
            header = nextHeader;
        }
    }

    Region::Region() noexcept
        : _block(nullptr), _newPageByteCount(DefaultNewPageByteCount)
    {
    }

    Region::Region(Region&& other) noexcept
        : _block(other._block), _newPageByteCount(other._newPageByteCount)
    {
        other._block = nullptr;
        other._newPageByteCount = DefaultNewPageByteCount;
    }

    Region::~Region() noexcept
    {
        Release();
    }

    Region& Region::operator=(Region&& other) noexcept
    {
        if (this != &other)
        {
            Release();
            _block = other._block;
            _newPageByteCount = other._newPageByteCount;

            other._block = nullptr;
            other._newPageByteCount = DefaultNewPageByteCount;
        }

        return *this;
    }

    void* Region::Allocate(int byteCount)
    {
        assert(byteCount > 0);

        auto header = (PageHeader*)_block;

        while (header && header->freeByteCount < byteCount)
            header = header->next;

        if (!header)
        {
            assert(_newPageByteCount > byteCount);
            header = (PageHeader*)malloc(_newPageByteCount);
            header->next = (PageHeader*)_block;
            header->freeBlock = (char*)(header + 1);
            header->pageByteCount = _newPageByteCount;
            header->freeByteCount = _newPageByteCount - sizeof(PageHeader);
            header->allocationCount = 0;

            _block = header;
        }

        void* result = header->freeBlock;
        header->freeBlock += byteCount;
        header->freeByteCount -= byteCount;
        ++header->allocationCount;
        return result;
    }

    int Region::PageCount() const noexcept
    {
        int result = 0;

        for (auto i = (PageHeader*)_block; i; i = i->next)
            ++result;

        return result;
    }

    void Region::DebugDump() const
    {
        auto header = (PageHeader*)_block;

        std::cout << "--- REGION " << (const void*)this << " ---";

        while (header)
        {
            std::cout << '\n' << header->pageByteCount << " byte page, ";
            std::cout << header->allocationCount << " allocation";
            if (header->allocationCount != 1) std::cout << 's';

            std::cout << ", " << header->freeByteCount << " byte";
            if (header->freeByteCount != 1) std::cout << 's';
            std::cout << " free";

            header = header->next;
        }

        std::cout << std::endl;
    }

    void Region::SetNewPageByteCount(int newPageByteCount)
    {
        _newPageByteCount =
            Max<int>(newPageByteCount, sizeof(PageHeader) * 16);
    }
}

void* operator new(size_t byteCount, Kelly::Region& region)
{
    return region.Allocate(byteCount);
}

void* operator new[](size_t byteCount, Kelly::Region& region)
{
    return region.Allocate(byteCount);
}
