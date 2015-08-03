#include "../include/Network/TcpSocket.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Kelly
{
    TcpSocket::TcpSocket() : _socket(-1)
    {
        _endpoint.address.networkValue = 0;
        _endpoint.port = 0;
    }

    TcpSocket::TcpSocket(const Endpoint32& endpoint)
        : _socket(-1), _endpoint(endpoint)
    {
        sockaddr_in sa = {};
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = _endpoint.address.networkValue;
        sa.sin_port = htons(endpoint.port);

        auto attempt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (attempt > 0)
        {
            auto result = connect(attempt, (const sockaddr*)&sa, sizeof(sa));

            if (result == -1)
            {
                close(attempt);
            }
            else
            {
                _socket = attempt;
            }
        }
    }

    TcpSocket::TcpSocket(TcpSocket&& other) : _socket(other._socket)
    {
        other._socket = -1;
    }

    TcpSocket::~TcpSocket()
    {
        Close();
    }

    void TcpSocket::Close()
    {
        if (_socket != -1) close(_socket);
    }

    bool TcpSocket::IsOpen() const
    {
        return _socket != -1;
    }

    void TcpSocket::Send(const View<const uint8_t>& data)
    {
        if (!IsOpen()) return;

        auto remaining = data.n;
        auto sent = remaining * 0;

        while (remaining > 0)
        {
            auto result = send(_socket, data.data + sent, remaining, 0);

            if (result != -1 && result > 0)
            {
                sent += result;
                remaining -= result;
            }
            else
            {
                Close();
                break;
            }
        }
    }

    ptrdiff_t TcpSocket::Receive(const View<uint8_t>& buffer)
    {
        if (!IsOpen()) return -1;
        return recv(_socket, buffer.data, buffer.n, 0);
    }
}
