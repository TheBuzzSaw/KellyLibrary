#ifndef TCPCONNECTION_HPP_KELLY
#define TCPCONNECTION_HPP_KELLY

#include "TcpSocket.hpp"

namespace Kelly
{
    class TcpConnection : public TcpSocket
    {
        public:
            TcpConnection();
            explicit TcpConnection(const Endpoint32& endpoint);
            TcpConnection(const TcpConnection&) = delete;
            TcpConnection(TcpConnection&& other);
            virtual ~TcpConnection();

            TcpConnection& operator=(const TcpConnection&) = delete;
            TcpConnection& operator=(TcpConnection&& other);

            void Send(const void* data, ptrdiff_t n);
            ptrdiff_t Receive(void* buffer, ptrdiff_t n);
    };
}

#endif
