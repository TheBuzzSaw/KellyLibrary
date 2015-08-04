#include "../include/Network/TcpSocket.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>

namespace Kelly
{
    TcpSocket::TcpSocket() : _socket(-1), _endpoint(NullEndpoint32)
    {
    }

    TcpSocket::TcpSocket(TcpSocket&& other)
        : _socket(other._socket), _endpoint(other._endpoint)
    {
        other._socket = -1;
        other._endpoint = NullEndpoint32;
    }

    TcpSocket::~TcpSocket()
    {
        Close();
    }

    TcpSocket& TcpSocket::operator=(TcpSocket&& other)
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

    void TcpSocket::Close()
    {
        if (_socket != -1)
        {
            close(_socket);
            _socket = -1;
        }
    }

    bool TcpSocket::IsOpen() const
    {
        return _socket != -1;
    }

    bool TcpSocket::SetBlocking(bool blocking)
    {
        if (!IsOpen()) return false;
        int flag = !blocking;
        return fcntl(_socket, F_SETFL, O_NONBLOCK, flag) != -1;
    }

    bool TcpSocket::SetDelay(bool delay)
    {
        if (!IsOpen()) return false;
        int flag = !delay;
        auto result = setsockopt(
            _socket,
            IPPROTO_TCP,
            TCP_NODELAY,
            (char*)&flag,
            sizeof(flag));

        return result >= 0;
    }
}
