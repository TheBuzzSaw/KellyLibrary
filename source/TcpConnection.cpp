#include "../include/Network/TcpConnection.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Kelly
{
    TcpConnection::TcpConnection() : TcpSocket()
    {
    }

    TcpConnection::TcpConnection(const Endpoint32& endpoint) : TcpSocket()
    {
        _endpoint = endpoint;
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

    TcpConnection::TcpConnection(TcpConnection&& other)
        : TcpSocket(std::move(other))
    {
    }

    TcpConnection::~TcpConnection()
    {
    }

    TcpConnection& TcpConnection::operator=(TcpConnection&& other)
    {
        return (TcpConnection&)TcpSocket::operator=(std::move(other));
    }

    void TcpConnection::Send(const void* data, ptrdiff_t n)
    {
        if (!IsOpen()) return;

        auto remaining = n;
        auto sent = remaining * 0;
        auto base = (const uint8_t*)data;

        while (remaining > 0)
        {
            auto result = send(_socket, base + sent, remaining, 0);

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

    ptrdiff_t TcpConnection::Receive(void* buffer, ptrdiff_t n)
    {
        if (!IsOpen()) return -1;
        return recv(_socket, buffer, n, 0);
    }
}
