#ifndef DATAMAP_HPP_KELLY
#define DATAMAP_HPP_KELLY

#include "View.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cassert>

namespace Kelly
{
    template<typename K, typename V> class DataMap
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
    {
        static constexpr ptrdiff_t KeyValueSize = sizeof(K) + sizeof(V);

        void* _block;
        ptrdiff_t _capacity;
        ptrdiff_t _count;

        void Expand(ptrdiff_t capacity)
        /// Private utility function to expand the memory block to be able to
        /// hold more key/value pairs. After resizing the block, any existing
        /// values are repositioned to correspond with the new capacity.
        ///
        /// capacity -- The minimum number of key/value pairs to make room for.
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

        V* FindValue(K key) const noexcept
        /// Private utility function to locate the address of a particular
        /// value based on a given key. This serves as the implementation for
        /// the const and non-const variants of TryGet.
        ///
        /// key -- The desired value's corresponding key.
        ///
        /// Returns a valid pointer to the value that corresponds with the key
        /// parameter. Otherwise returns null indicating the key was not found.
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

        V& Insert(K key, ptrdiff_t position)
        /// Private utility function to create a new key/value pair. It is
        /// assumed that this function's caller already did the work to
        /// determine where in the collection the insertion needs to take
        /// place. Used in AlwaysGet, Add, and Set.
        ///
        /// key -- The new pair's key.
        /// position -- The collection ordinal to begin shifting elements.
        ///
        /// Returns a reference to the newly created pair's value.
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

        DataMap() noexcept
        /// Constructs an empty DataMap object. No memory is allocated until a
        /// key/value pair is added.
            : _block(nullptr)
            , _capacity(0)
            , _count(0)
        {
        }

        DataMap(DataMap&& other) noexcept
        /// Move-constructs a DataMap object. The other DataMap object is left
        /// with no allocated memory (as if it were newly constructed using
        /// DataMap()).
        ///
        /// other -- DataMap object to have its content taken.
            : _block(other._block)
            , _capacity(other._capacity)
            , _count(other._count)
        {
            other._block = nullptr;
            other._capacity = 0;
            other._count = 0;
        }

        DataMap(const DataMap& other)
        /// Copy-constructs a DataMap object. Memory is only allocated if the
        /// other DataMap object has key/value pairs in it. All corresponding
        /// pairs are copied over in that case.
        ///
        /// other -- DataMap object to have its content copied.
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

        ~DataMap() noexcept
        /// Destroys this DataMap object. Any allocated memory is released as a
        /// single block. No other destructors are called as this is a
        /// POD-focused collection.
        {
            free(_block);
        }

        DataMap& operator=(DataMap&& other) noexcept
        /// Move-assigns another DataMap object. This object's allocated memory
        /// is unconditionally freed before assimilating the other object's
        /// content. The other DataMap object is left with no allocated memory
        /// (as if it were newly constructed using DataMap()).
        ///
        /// other -- DataMap object to have its content taken.
        ///
        /// Returns a reference to this object.
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
        /// Copy-assigns another DataMap object. If this object already has
        /// sufficient capacity to copy the other object's content, the data is
        /// is simply copied. Otherwise, this object's memory is resized to
        /// correspond to the other object's key/value pair count.
        ///
        /// other -- DataMap object to have its content copied.
        ///
        /// Returns a reference to this object.
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
        /// Returns the number of bytes occupied by this collection's memory
        /// block.
        {
            return _capacity * KeyValueSize;
        }

        ptrdiff_t Capacity() const noexcept
        /// Returns the number of key/value pairs this collection is capable of
        /// holding.
        {
            return _capacity;
        }

        ptrdiff_t Count() const noexcept
        /// Returns the number of key/value pairs currently in this collection.
        {
            return _count;
        }

        void Clear() noexcept
        /// Removes all key/value pairs from this collection leaving it with a
        /// count of zero. No memory is freed.
        {
            _count = 0;
        }

        V* TryGet(K key) noexcept
        /// Safely attempts to obtain the value corresponding to the given key.
        /// This collection remains unmodified regardless of the result of this
        /// operation.
        ///
        /// key -- The desired value's corresponding key.
        ///
        /// Returns a valid pointer to the value that corresponds with the key
        /// parameter. Otherwise returns null indicating the key was not found.
        {
            return FindValue(key);
        }

        const V* TryGet(K key) const noexcept
        /// Safely attempts to obtain the value corresponding to the given key.
        /// This collection remains unmodified regardless of the result of this
        /// operation.
        ///
        /// key -- The desired value's corresponding key.
        ///
        /// Returns a valid pointer to the read-only value that corresponds
        /// with the key parameter. Otherwise returns null indicating the key
        /// was not found.
        {
            return FindValue(key);
        }

        V& AlwaysGet(K key)
        /// Obtains the value corresponding to the given key. This operation
        /// always succeeds (barring any memory-allocation failures). If the
        /// key is found, the corresponding value is returned. Otherwise, the
        /// key is inserted, and the newly created value is returned.
        ///
        /// key -- The desired value's corresponding key.
        ///
        /// Returns a reference to the value that corresponds with the key
        /// parameter.
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
        /// Attempts to add a new key (and value) to this collection. The
        /// assumption is that the specified key is currently absent.
        ///
        /// key -- The key to be created.
        /// value -- The value to be created corresponding to the key.
        ///
        /// Returns true if the key and value were successfully added to this
        /// collection. Otherwise returns false indicating that the key
        /// already existed and that its corresponding value was not modified.
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
        /// Sets the value corresponding to the given key. This operation
        /// always succeeds (barring any memory-allocation failures). If the
        /// key is found, the corresponding value is modified. Otherwise, the
        /// key is inserted, and the corresponding value is inserted.
        ///
        /// key -- The desired value's corresponding key.
        /// value -- The value to be created or modified.
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
        /// Attempts to remove a key (and its corresponding value) from this
        /// collection. If the key is located, the pair is removed. Otherwise,
        /// no action is taken. Both paths result in this collection having no
        /// key matching the parameter.
        ///
        /// key -- The key to be removed (along with its corresponding value).
        ///
        /// Returns true if the key was successfully located and removed (along
        /// with its corresponding value). Otherwise, returns false indicating
        /// that the key was absent.
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
        /// Sets a minimum capacity for this collection. If the requested
        /// capacity is larger than the current capacity, the memory block is
        /// expanded to accommodate the new minimum. This is useful in
        /// avoiding unnecessary expansions for when a known quantity of keys
        /// and values are about to be inserted.
        ///
        /// capacity -- The minimum number of key/value pairs this collection
        /// needs to be able to hold.
        {
            if (capacity > _capacity) Expand(capacity);
        }

        View<V> Values() noexcept
        /// Returns an iterable collection of all values in this collection.
        /// The returned collection is invalidated by any function that
        /// modifies this object.
        {
            return { (V*)((K*)_block + _capacity), _count };
        }

        View<const K> Keys() const noexcept
        /// Returns an iterable collection of all keys in this collection.
        /// The returned collection is invalidated by any function that
        /// modifies this object.
        {
            return { (K*)_block, _count };
        }

        View<const V> Values() const noexcept
        /// Returns an iterable read-only collection of all values in this
        /// collection. The returned collection is invalidated by any function
        /// that modifies this object.
        {
            return { (V*)((K*)_block + _capacity), _count };
        }
    };
}

#endif

