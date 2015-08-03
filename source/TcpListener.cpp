#include "../include/Network/TcpListener.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Kelly
{
    TcpListener::TcpListener() : TcpSocket()
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
        : TcpSocket(std::move(other))
    {
    }

    TcpListener::~TcpListener()
    {
    }

    TcpListener& TcpListener::operator=(TcpListener&& other)
    {
        return (TcpListener&)TcpSocket::operator=(std::move(other));
    }

    TcpConnection TcpListener::Accept() const
    {
        TcpConnection result;
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
