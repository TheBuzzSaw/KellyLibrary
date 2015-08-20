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
        static constexpr auto KeyValueSize = sizeof(K) + sizeof(V);

        void* _block;
        int _capacity;
        int _count;

        void Expand(int capacity)
        {
            auto proposedCapacity = Max(capacity, 8);

            _block = realloc(_block, proposedCapacity * KeyValueSize);
            assert(_block != nullptr);

            memmove(
                (K*)_block + proposedCapacity,
                (K*)_block + _capacity,
                sizeof(V) * (size_t)_count);

            _capacity = proposedCapacity;
        }

    public:
        DataMap() noexcept
            : _block(nullptr)
            , _capacity(0)
            , _count(0)
        {
        }

        DataMap(DataMap&& other) noexcept
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
            else
            {
                _block = nullptr;
            }
        }

        ~DataMap() noexcept
        {
            free(_block);
        }

        DataMap& operator=(DataMap&& other) noexcept
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

        inline int Capacity() const noexcept { return _capacity; }
        inline int Count() const noexcept { return _count; }
        inline void Clear() noexcept { _count = 0; }

        V* TryGet(K key) noexcept
        {
            auto keys = (K*)_block;
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);

            if (k != endKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                auto distance = k - keys;
                return values + distance;
            }

            return nullptr;
        }

        V& AlwaysGet(K key)
        {
            auto keys = (K*)_block;
            auto values = (V*)(keys + _capacity);
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);
            auto distance = k - keys;

            if (k != endKey && *k == key)
            {
                return values[distance];
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
                return *valueSlot;
            }
        }

        void Set(K key, V value)
        {
            auto keys = (K*)_block;
            auto values = (V*)(keys + _capacity);
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);
            auto distance = k - keys;

            if (k != endKey && *k == key)
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

        bool Remove(K key) noexcept
        {
            auto keys = (K*)_block;
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);

            if (k != endKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                auto distance = k - keys;
                auto moveCount = --_count - distance;

                memmove(k, k + 1, moveCount * sizeof(K));

                auto valueSlot = values + distance;
                memmove(valueSlot, valueSlot + 1, moveCount * sizeof(V));
                return true;
            }

            return false;
        }

        void Reserve(int capacity)
        {
            if (capacity > _capacity) Expand(capacity);
        }

        View<K> Keys() noexcept
        {
            return { (K*)_block, _count };
        }

        View<V> Values() noexcept
        {
            return { (V*)((K*)_block + _capacity), _count };
        }

        View<const K> ConstKeys() const noexcept
        {
            return { (K*)_block, _count };
        }

        View<const V> ConstValues() const noexcept
        {
            return { (V*)((K*)_block + _capacity), _count };
        }
    };
}

#endif

