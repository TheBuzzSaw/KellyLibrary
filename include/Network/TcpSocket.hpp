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

        TcpSocket(const TcpSocket&) = delete;
        TcpSocket(TcpSocket&& other);

        TcpSocket& operator=(const TcpSocket&) = delete;
        TcpSocket& operator=(TcpSocket&& other);

    public:
        TcpSocket();
        virtual ~TcpSocket();

        inline Endpoint32 Endpoint() const { return _endpoint; }

        void Close();
        bool IsOpen() const;
        bool SetBlocking(bool blocking);

        friend class TcpListener;
        friend class TcpConnection;
    };
}

#endif
