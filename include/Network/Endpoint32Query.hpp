#ifndef ENDPOINT32QUERY_HPP_KELLY
#define ENDPOINT32QUERY_HPP_KELLY

#include "Endpoint32.hpp"

namespace Kelly
{
    class Endpoint32QueryIterator
    {
        void* _data = nullptr;

        explicit constexpr Endpoint32QueryIterator(void* data) : _data(data) {}

    public:
        constexpr Endpoint32QueryIterator() = default;
        constexpr Endpoint32QueryIterator(const Endpoint32QueryIterator&)
            = default;
        ~Endpoint32QueryIterator() = default;

        Endpoint32QueryIterator& operator=(const Endpoint32QueryIterator&)
            = default;

        Endpoint32 operator*() const;
        Endpoint32QueryIterator& operator++();
        Endpoint32QueryIterator operator++(int);

        constexpr bool operator==(const Endpoint32QueryIterator& other)
        {
            return _data == other._data;
        }

        constexpr bool operator!=(const Endpoint32QueryIterator& other)
        {
            return _data != other._data;
        }

        friend class Endpoint32Query;
    };

    class Endpoint32Query
    {
        void* _data = nullptr;

    public:
        constexpr Endpoint32Query() = default;
        Endpoint32Query(const char* address, const char* port);
        Endpoint32Query(const Endpoint32Query&) = delete;
        Endpoint32Query(Endpoint32Query&& other);
        ~Endpoint32Query();

        Endpoint32Query& operator=(const Endpoint32Query&) = delete;
        Endpoint32Query& operator=(Endpoint32Query&& other);

        inline Endpoint32QueryIterator begin() const
        {
            return Endpoint32QueryIterator(_data);
        }

        inline Endpoint32QueryIterator end() const
        {
            return Endpoint32QueryIterator();
        }
    };
}

#endif
