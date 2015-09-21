#ifndef STRUCTUREOFARRAYS_HPP_KELLY
#define STRUCTUREOFARRAYS_HPP_KELLY

#include "View.hpp"
#include <initializer_list>
#include <cassert>
#include <cstring>
#include <cstddef>
#include <cstdlib>

namespace Kelly
{
    template<int N> class StructureOfArrays
    {
        int _recordByteCount;
        int _byteCounts[N];
        int _offsets[N];
        void* _block;
        ptrdiff_t _capacity;
        ptrdiff_t _count;

        void* GetBlock(int n) const
        {
            assert(InRange(n, 0, N - 1));
            return (char*)_block + _offsets[n] * _capacity;
        }

    public:
        StructureOfArrays(std::initializer_list<int> byteCounts) noexcept
            : _recordByteCount(0)
            , _block(nullptr)
            , _capacity(0)
            , _count(0)
        {
            assert(byteCounts.size() == N);

            for (int i = 0; i < N; ++i)
            {
                _byteCounts[i] = byteCounts.begin()[i];
                _recordByteCount += _byteCounts[i];
            }

            _offsets[0] = 0;

            for (int i = 1; i < N; ++i)
                _offsets[i] = _offsets[i - 1] + _byteCounts[i - 1];
        }

        StructureOfArrays(StructureOfArrays&& other) noexcept
        {
            memcpy(this, &other, sizeof(*this));

            other._block = nullptr;
            other._capacity = 0;
            other._count = 0;
        }

        StructureOfArrays(const StructureOfArrays& other)
        {
        }

        ~StructureOfArrays() noexcept
        {
            free(_block);
        }

        StructureOfArrays& operator=(StructureOfArrays&& other) noexcept;
        StructureOfArrays& operator=(const StructureOfArrays& other);

        inline ptrdiff_t Capacity() const { return _capacity; }
        inline ptrdiff_t Count() const { return _count; }

        void Resize(ptrdiff_t n)
        {
            if (n < _capacity)
            {
                _count = Max<ptrdiff_t>(n, 0);
                return;
            }

            ptrdiff_t proposedCapacity = Max<ptrdiff_t>(_capacity * 2, 8);
            while (proposedCapacity < n) proposedCapacity *= 2;
            void* block = realloc(_block, proposedCapacity * _recordByteCount);
            assert(block != nullptr);
            _block = block;

            for (int i = N - 1; i > 0; --i)
            {
                auto source = (char*)_block + _offsets[i] * _capacity;
                auto destination =
                    (char*)_block + _offsets[i] * proposedCapacity;

                memmove(destination, source, _byteCounts[i] * _count);
            }

            _capacity = proposedCapacity;
            _count = n;
        }

        void Grow(ptrdiff_t n)
        {
            Resize(_count + n);
        }

        void Shrink(ptrdiff_t n)
        {
            Resize(_count - n);
        }

        template<typename T> View<T> GetArray(int n)
        {
            assert(sizeof(T) == _byteCounts[n]);
            return { (T*)GetBlock(n), _count };
        }

        template<typename T> View<const T> GetArray(int n) const
        {
            assert(sizeof(T) == _byteCounts[n]);
            return { (const T*)GetBlock(n), _count };
        }
    };
}

#endif
