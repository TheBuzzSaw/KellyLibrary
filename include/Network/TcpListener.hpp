#ifndef TCPLISTENER_HPP_KELLY
#define TCPLISTENER_HPP_KELLY

#include "TcpSocket.hpp"

namespace Kelly
{
    class TcpListener
    {
        int _socket;
        Endpoint32 _endpoint;

    public:
        TcpListener();
        explicit TcpListener(uint16_t port);
        TcpListener(const TcpListener&) = delete;
        TcpListener(TcpListener&& other);
        ~TcpListener();

        TcpListener& operator=(const TcpListener&) = delete;
        TcpListener& operator=(TcpListener&& other);

        inline Endpoint32 Endpoint() const { return _endpoint; }

        void Close();
        bool IsOpen() const;
        bool SetBlocking(bool blocking);

        TcpSocket Accept() const;
    };
}

#endif
