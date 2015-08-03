#include "../include/Network/TcpListener.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

namespace Kelly
{
    TcpListener::TcpListener() : _socket(-1), _endpoint(NullEndpoint32)
    {
    }

    TcpListener::TcpListener(uint16_t port) : TcpListener()
    {
        _endpoint.port = port;
        auto attempt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (attempt == -1) return;

        int yes = 1;
        setsockopt(
            attempt,
            SOL_SOCKET,
            SO_REUSEADDR,
            &yes,
            sizeof(yes));

        sockaddr_in service = {};
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = LocalHost32.networkValue;
        service.sin_port = htons(port);

        auto result = bind(attempt, (sockaddr*)&service, sizeof(service));

        if (result == -1)
        {
            close(attempt);
        }
        else
        {
            _socket = attempt;
        }
    }

    TcpListener::TcpListener(TcpListener&& other)
        : _socket(other._socket), _endpoint(other._endpoint)
    {
        other._socket = -1;
        other._endpoint = NullEndpoint32;
    }

    TcpListener::~TcpListener()
    {
        Close();
    }

    TcpListener& TcpListener::operator=(TcpListener&& other)
    {
        if (this != &other)
        {
            _socket = other._socket;
            _endpoint = other._endpoint;
            other._socket = -1;
            other._endpoint = NullEndpoint32;
        }

        return *this;
    }

    void TcpListener::Close()
    {
        if (_socket != -1)
        {
            close(_socket);
            _socket = -1;
        }
    }

    bool TcpListener::IsOpen() const
    {
        return _socket != -1;
    }

    bool TcpListener::SetBlocking(bool blocking)
    {
        int nonBlocking = !blocking;
        return fcntl(_socket, F_SETFL, O_NONBLOCK, nonBlocking) != -1;
    }

    TcpSocket TcpListener::Accept() const
    {
        TcpSocket result;
        if (!IsOpen()) return result;

        if (listen(_socket, 2)) return result;

        sockaddr_in from = {};
        socklen_t fromLen = sizeof(from);
        result._socket = accept(_socket, (sockaddr*)&from, &fromLen);
        result._endpoint.address.networkValue = from.sin_addr.s_addr;
        result._endpoint.port = ntohs(from.sin_port);

        return result;
    }
}
