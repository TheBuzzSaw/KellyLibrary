#ifndef TCPLISTENER_HPP_KELLY
#define TCPLISTENER_HPP_KELLY

#include "TcpConnection.hpp"

namespace Kelly
{
    class TcpListener : public TcpSocket
    {
    public:
        TcpListener();
        explicit TcpListener(uint16_t port);
        TcpListener(const TcpListener&) = delete;
        TcpListener(TcpListener&& other);
        virtual ~TcpListener();

        TcpListener& operator=(const TcpListener&) = delete;
        TcpListener& operator=(TcpListener&& other);

        TcpConnection Accept() const;
    };
}

#endif
