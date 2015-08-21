#ifndef SPLITARRAY_HPP_KELLY
#define SPLITARRAY_HPP_KELLY

#include "Math.hpp"

namespace Kelly
{
    template<typename Ta, typename Tb> class SplitArray
    {
        static constexpr auto UnitSize = sizeof(Ta) + sizeof(Tb);

        void* _block;
        int _capacity;
        int _count;

        void Expand(int capacity)
        {
            auto proposedCapacity = Max(capacity, 8);

            _block = realloc(_block, proposedCapacity * UnitSize);
            assert(_block != nullptr);

            memmove(
                (Ta*)_block + proposedCapacity,
                (Ta*)_block + _capacity,
                sizeof(Tb) * (size_t)_count);

            _capacity = proposedCapacity;
        }

    public:
        SplitArray() noexcept
            : _block(nullptr)
            , _capacity(0)
            , _count(0)
        {
        }

        SplitArray(SplitArray&& other) noexcept
            : _block(other._block)
            , _capacity(other._capacity)
            , _count(other._count)
        {
            other._block = nullptr;
            other._capacity = 0;
            other._count = 0;
        }

        SplitArray(const SplitArray& other)
            : _capacity(other._capacity)
            , _count(other._count)
        {
            if (_capacity > 0)
            {
                auto totalSize = (size_t)_capacity * KeyValueSize;
                _block = malloc(totalSize);
                memcpy(_block, other._block, totalSize);
            }
            else
            {
                _block = nullptr;
            }
        }

        ~SplitArray()
        {
            free(_block);
        }

        SplitArray& operator=(SplitArray&& other) noexcept
        {
            if (this != &other)
            {
                this->~SplitArray();
                new (this) SplitArray(std::move(other));
            }

            return *this;
        }

        SplitArray& operator=(const SplitArray& other)
        {
            if (this != &other)
            {
                this->~SplitArray();
                new (this) SplitArray(other);
            }

            return *this;
        }

        inline int Capacity() const noexcept { return _capacity; }
        inline int Count() const noexcept { return _count; }
        inline void Clear() noexcept { _count = 0; }

        void Reserve(int capacity)
        {
            if (capacity > _capacity) Expand(capacity);
        }

        View<Ta> Heads() noexcept
        {
            return { (Ta*)_block, _count };
        }

        View<Tb> Tails() noexcept
        {
            return { (Tb*)((Ta*)_block + _capacity), _count };
        }

        View<const Ta> ConstHeads() const noexcept
        {
            return { (Ta*)_block, _count };
        }

        View<const Tb> ConstTails() const noexcept
        {
            return { (Tb*)((Ta*)_block + _capacity), _count };
        }
    };
}

#endif
