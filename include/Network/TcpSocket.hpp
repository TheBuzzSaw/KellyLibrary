#ifndef TCPSOCKET_HPP_KELLY
#define TCPSOCKET_HPP_KELLY

#include "Endpoint32.hpp"
#include "../View.hpp"

namespace Kelly
{
    class TcpSocket
    {
        int _socket;
        Endpoint32 _endpoint;

    public:
        TcpSocket();
        explicit TcpSocket(const Endpoint32& endpoint);
        TcpSocket(const TcpSocket&) = delete;
        TcpSocket(TcpSocket&& other);
        ~TcpSocket();

        TcpSocket& operator=(const TcpSocket&) = delete;
        TcpSocket& operator=(TcpSocket&& other);

        inline Endpoint32 Endpoint() const { return _endpoint; }

        void Close();
        bool IsOpen() const;

        void Send(const View<const uint8_t>& data);
        ptrdiff_t Receive(const View<uint8_t>& buffer);
    };
}

#endif
