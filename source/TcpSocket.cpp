#include "../include/Network/TcpSocket.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
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
        int nonBlocking = !blocking;
        return fcntl(_socket, F_SETFL, O_NONBLOCK, nonBlocking) != -1;
    }
}
