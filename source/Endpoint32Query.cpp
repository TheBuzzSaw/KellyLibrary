#include "../include/Network/Endpoint32Query.hpp"
#include "../include/Tools.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Kelly
{
    Endpoint32 Endpoint32QueryIterator::operator*() const
    {
        auto info = (addrinfo*)_data;
        auto address = (sockaddr_in*)info->ai_addr;

        Endpoint32 result;
        result.address.networkValue = address->sin_addr.s_addr;
        result.port = ntohs(address->sin_port);
        return result;
    }

    Endpoint32QueryIterator& Endpoint32QueryIterator::operator++()
    {
        auto info = (addrinfo*)_data;
        _data = info->ai_next;
        return *this;
    }

    Endpoint32QueryIterator Endpoint32QueryIterator::operator++(int)
    {
        auto result = *this;
        auto info = (addrinfo*)_data;
        _data = info->ai_next;
        return result;
    }

    Endpoint32Query::Endpoint32Query(const char* address, const char* port)
    {
        if (!HasContent(address) || !HasContent(port)) return;

        addrinfo* result = nullptr;
        addrinfo hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        if (getaddrinfo(address, port, &hints, &result) == 0)
            _data = result;
    }

    Endpoint32Query::Endpoint32Query(Endpoint32Query&& other)
        : _data(other._data)
    {
        other._data = nullptr;
    }

    Endpoint32Query::~Endpoint32Query()
    {
        if (_data) freeaddrinfo((addrinfo*)_data);
    }

    Endpoint32Query& Endpoint32Query::operator=(Endpoint32Query&& other)
    {
        if (this != &other)
        {
            if (_data) freeaddrinfo((addrinfo*)_data);
            _data = other._data;
            other._data = nullptr;
        }

        return *this;
    }

    Endpoint32 FindEndpoint32(const char* address, const char* port)
    {
        auto result = NullEndpoint32;
        Endpoint32Query query(address, port);
        if (query.begin() != query.end()) result = *query.begin();
        return result;
    }
}
