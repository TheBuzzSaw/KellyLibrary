#ifndef DATAMAP_HPP_KELLY
#define DATAMAP_HPP_KELLY

#include "Math.hpp"
#include "View.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cassert>

namespace Kelly
{
    template<typename K, typename V> class DataMap
    {
        void* _block;
        int _capacity;
        int _count;

        void Expand(int capacity)
        {
            auto proposedCapacity = Max(capacity, 8);

            _block = realloc(_block, proposedCapacity * KeyValueSize);
            assert(_block);

            memmove(
                (K*)_block + proposedCapacity,
                (K*)_block + _capacity,
                sizeof(V) * size_t(_count));

            _capacity = proposedCapacity;
        }

    public:
        static constexpr auto KeyValueSize = sizeof(K) + sizeof(V);

        DataMap()
            : _block(nullptr)
            , _capacity(0)
            , _count(0)
        {
        }

        DataMap(DataMap&& other)
            : _block(other._block)
            , _capacity(other._capacity)
            , _count(other._count)
        {
            other._block = nullptr;
            other._capacity = 0;
            other._count = 0;
        }

        DataMap(const DataMap& other)
            : _capacity(other._capacity)
            , _count(other._count)
        {
            if (_capacity > 0)
            {
                auto totalSize = (size_t)_capacity * KeyValueSize;
                _block = malloc(totalSize);
                memcpy(_block, other._block, totalSize);
            }
        }

        ~DataMap()
        {
            free(_block);
        }

        DataMap& operator=(DataMap&& other)
        {
            if (this != &other)
            {
                this->~DataMap();
                new (this) DataMap(std::move(other));
            }

            return *this;
        }

        DataMap& operator=(const DataMap& other)
        {
            if (this != &other)
            {
                this->~DataMap();
                new (this) DataMap(other);
            }

            return *this;
        }

        inline int Capacity() const { return _capacity; }
        inline int Count() const { return _count; }
        inline void Clear() { _count = 0; }

        V* Get(K key)
        {
            auto keys = (K*)_block;
            auto lastKey = keys + _count;

            auto k = std::lower_bound(keys, lastKey, key);

            if (k != lastKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                auto distance = k - keys;
                return values + distance;
            }

            return nullptr;
        }

        void Set(K key, V value)
        {
            auto keys = (K*)_block;
            auto values = (V*)(keys + _capacity);
            auto lastKey = keys + _count;

            auto k = std::lower_bound(keys, lastKey, key);
            auto distance = k - keys;

            if (k != lastKey && *k == key)
            {
                values[distance] = value;
            }
            else
            {
                if (_count == _capacity)
                {
                    Expand(_capacity * 2);
                    keys = (K*)_block;
                    values = (V*)(keys + _capacity);
                }

                auto moveCount = _count++ - distance;
                auto keySlot = keys + distance;
                memmove(keySlot + 1, keySlot, moveCount * sizeof(K));
                *keySlot = key;

                auto valueSlot = values + distance;
                memmove(valueSlot + 1, valueSlot, moveCount * sizeof(V));
                *valueSlot = value;
            }
        }

        void Remove(K key)
        {
            auto keys = (K*)_block;
            auto lastKey = keys + _count;

            auto k = std::lower_bound(keys, lastKey, key);

            if (k != lastKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                auto distance = k - keys;
                auto moveCount = --_count - distance;

                memmove(k, k + 1, moveCount * sizeof(K));

                auto valueSlot = values + distance;
                memmove(valueSlot, valueSlot + 1, moveCount * sizeof(V));
            }
        }

        void Reserve(int capacity)
        {
            if (capacity > _capacity) Expand(capacity);
        }

        View<K> Keys() const
        {
            return { (K*)_block, _count };
        }

        View<V> Values() const
        {
            return { (V*)((K*)_block + _capacity), _count };
        }
    };
}

#endif

