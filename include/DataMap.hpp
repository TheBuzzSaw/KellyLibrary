#ifndef DATAMAP_HPP_KELLY
#define DATAMAP_HPP_KELLY

#include "View.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cassert>

namespace Kelly
{
    /// DataMap is a data-oriented associative array. Its focus is on POD
    /// types, which means DataMap never invokes the keys' or values'
    /// respective constructors, destructors, or assignment operators. The data
    /// is laid out in a cache-friendly manner. Whereas traditional map
    /// implementations often store key/value pairs together (KVKVKVKV),
    /// DataMap keeps keys and values in separate containers (KKKKVVVV). This
    /// provides several key tradeoffs:
    ///
    /// (1) Keys are packed in a cache-friendly manner. While not as fast as a
    /// hash-based lookup, binary searches here are significantly faster than
    /// traversing a cache-hostile binary tree.
    ///
    /// (2) Keys are kept in order. This makes insertion generally slower, but
    /// if the keys are ever incrementing, pairs are always inserted on the
    /// back, which ends up being significantly faster. Insertion and removal
    /// near the front is the worst case.
    ///
    /// (3) Keys and values can be traversed separately. Values are packed in a
    /// cache-friendly way the same as keys are. DataMap exposes functions for
    /// accessing the two respective arrays for iteration. This positions
    /// DataMap to be extremely powerful for cases where insertion/removal are
    /// infrequent and where iteration happens often.
    template<typename K, typename V> class DataMap
    {
        static constexpr ptrdiff_t KeyValueSize = sizeof(K) + sizeof(V);

        void* _block;
        ptrdiff_t _capacity;
        ptrdiff_t _count;

        /// Private utility function to expand the memory block to be able to
        /// hold more key/value pairs. After resizing the block, any existing
        /// values are repositioned to correspond with the new capacity.
        ///
        /// capacity -- The minimum number of key/value pairs to make room for.
        void Expand(ptrdiff_t capacity)
        {
            auto proposedCapacity = std::max<ptrdiff_t>(capacity, 8);

            void* block = realloc(_block, proposedCapacity * KeyValueSize);
            assert(block != nullptr);
            _block = block;

            memmove(
                (K*)_block + proposedCapacity,
                (K*)_block + _capacity,
                sizeof(V) * (size_t)_count);

            _capacity = proposedCapacity;
        }

        /// Private utility function to locate the address of a particular
        /// value based on a given key. This serves as the implementation for
        /// the const and non-const variants of TryGet.
        ///
        /// key -- The desired value's corresponding key.
        ///
        /// Returns a valid pointer to the value that corresponds with the key
        /// parameter. Otherwise returns null indicating the key was not found.
        V* FindValue(K key) const noexcept
        {
            auto keys = (K*)_block;
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);

            if (k != endKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                auto position = k - keys;
                return values + position;
            }

            return nullptr;
        }

        /// Private utility function to create a new key/value pair. It is
        /// assumed that this function's caller already did the work to
        /// determine where in the collection the insertion needs to take
        /// place. Used in AlwaysGet, Add, and Set.
        ///
        /// key -- The new pair's key.
        /// position -- The collection ordinal to begin shifting elements.
        ///
        /// Returns a reference to the newly created pair's value.
        V& Insert(K key, ptrdiff_t position)
        {
            if (_count == _capacity) Expand(_capacity * 2);
            auto keys = (K*)_block;
            auto values = (V*)(keys + _capacity);

            auto moveCount = _count++ - position;
            auto keySlot = keys + position;
            memmove(keySlot + 1, keySlot, moveCount * sizeof(K));
            *keySlot = key;

            auto valueSlot = values + position;
            memmove(valueSlot + 1, valueSlot, moveCount * sizeof(V));
            return *valueSlot;
        }

    public:

        /// Constructs an empty DataMap object. No memory is allocated until a
        /// key/value pair is added.
        DataMap() noexcept
            : _block(nullptr)
            , _capacity(0)
            , _count(0)
        {
        }

        /// Move-constructs a DataMap object. The other DataMap object is left
        /// with no allocated memory (as if it were newly constructed using
        /// DataMap()).
        ///
        /// other -- DataMap object to have its content taken.
        DataMap(DataMap&& other) noexcept
            : _block(other._block)
            , _capacity(other._capacity)
            , _count(other._count)
        {
            other._block = nullptr;
            other._capacity = 0;
            other._count = 0;
        }

        /// Copy-constructs a DataMap object. Memory is only allocated if the
        /// other DataMap object has key/value pairs in it. All corresponding
        /// pairs are copied over in that case.
        ///
        /// other -- DataMap object to have its content copied.
        DataMap(const DataMap& other)
            : DataMap()
        {
            if (other._count > 0)
            {
                Expand(other._count);
                _count = other._count;

                memcpy(
                    _block,
                    other._block,
                    (size_t)_count * sizeof(K));

                memcpy(
                    (K*)_block + _capacity,
                    (K*)other._block + other._capacity,
                    (size_t)_count * sizeof(V));
            }
        }

        /// Destroys this DataMap object. Any allocated memory is released as a
        /// single block. No other destructors are called as this is a
        /// POD-focused collection.
        ~DataMap() noexcept
        {
            free(_block);
        }

        DataMap& operator=(DataMap&& other) noexcept
        {
            if (this != &other)
            {
                free(_block);

                _block = other._block;
                _capacity = other._capacity;
                _count = other._count;

                other._block = nullptr;
                other._capacity = 0;
                other._count = 0;
            }

            return *this;
        }

        DataMap& operator=(const DataMap& other)
        {
            if (this != &other)
            {
                if (_capacity < other._count)
                {
                    _count = 0;
                    Expand(other._count);
                }

                _count = other._count;

                memcpy(
                    _block,
                    other._block,
                    (size_t)_count * sizeof(K));

                memcpy(
                    (K*)_block + _capacity,
                    (K*)other._block + other._capacity,
                    (size_t)_count * sizeof(V));
            }

            return *this;
        }

        ptrdiff_t RawSize() const noexcept
        {
            return _capacity * KeyValueSize;
        }

        ptrdiff_t Capacity() const noexcept
        {
            return _capacity;
        }

        ptrdiff_t Count() const noexcept
        {
            return _count;
        }

        void Clear() noexcept
        {
            _count = 0;
        }

        V* TryGet(K key) noexcept
        {
            return FindValue(key);
        }

        const V* TryGet(K key) const noexcept
        {
            return FindValue(key);
        }

        V& AlwaysGet(K key)
        {
            auto keys = (K*)_block;
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);
            auto position = k - keys;

            if (k != endKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                return values[position];
            }

            return Insert(key, position);
        }

        bool Add(K key, V value)
        {
            auto keys = (K*)_block;
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);

            if (k != endKey && *k == key) return false;

            auto position = k - keys;
            Insert(key, position) = value;
            return true;
        }

        void Set(K key, V value)
        {
            auto keys = (K*)_block;
            auto endKey = keys + _count;

            auto k = std::lower_bound(keys, endKey, key);
            auto position = k - keys;

            if (k != endKey && *k == key)
            {
                auto values = (V*)(keys + _capacity);
                values[position] = value;
            }
            else
            {
                Insert(key, position) = value;
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
                auto position = k - keys;
                auto moveCount = --_count - position;

                memmove(k, k + 1, (size_t)moveCount * sizeof(K));

                auto valueSlot = values + position;
                memmove(valueSlot, valueSlot + 1, moveCount * sizeof(V));
                return true;
            }

            return false;
        }

        void Reserve(ptrdiff_t capacity)
        {
            if (capacity > _capacity) Expand(capacity);
        }

        View<V> Values() noexcept
        {
            return { (V*)((K*)_block + _capacity), _count };
        }

        View<const K> Keys() const noexcept
        {
            return { (K*)_block, _count };
        }

        View<const V> Values() const noexcept
        {
            return { (V*)((K*)_block + _capacity), _count };
        }
    };
}

#endif

