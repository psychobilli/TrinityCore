#include "AsyncAcceptor.h"
#include "MoqAsyncAcceptor.h"
#include "WorldSocket.h"
#include "MoqWorldSocketMgr.h"
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <atomic>

std::shared_ptr<WorldSocket> MoqAsyncAcceptor::BuildWorldSocket() {
    std::shared_ptr<WorldSocket> socket = sMoqWorldSocketMgr.GetSocket();
    if (!socket)
    {
        sMoqWorldSocketMgr.OnSocketOpen(std::move(this->_socket), 0);
        //_acceptor.async_accept(_socket, [this](boost::system::error_code error)
        //{
        //    if (!error)
        //    {
        //        try
        //        {
        //            // this-> is required here to fix an segmentation fault in gcc 4.7.2 - reason is lambdas in a templated class
        //            std::make_shared<WorldSocket>(std::move(this->_socket))->Start();
        //        }
        //        catch (boost::system::system_error const& err)
        //        {
        //            TC_LOG_INFO("network", "Failed to retrieve client's remote address %s", err.what());
        //        }
        //    }

        //    // lets slap some more this-> on this so we can fix this bug with gcc 4.7.2 throwing internals in yo face
        //    if (!_closed)
        //        this->AsyncAccept<WorldSocket>();
        //});
        socket = sMoqWorldSocketMgr.GetSocket();
    }
    return socket;
}

//template<class T>
//void AsyncAcceptor::AsyncAccept()
//{
//    _acceptor.async_accept(_socket, [this](boost::system::error_code error)
//    {
//        if (!error)
//        {
//            try
//            {
//                // this-> is required here to fix an segmentation fault in gcc 4.7.2 - reason is lambdas in a templated class
//                std::make_shared<T>(std::move(this->_socket))->Start();
//            }
//            catch (boost::system::system_error const& err)
//            {
//                TC_LOG_INFO("network", "Failed to retrieve client's remote address %s", err.what());
//            }
//        }
//
//        // lets slap some more this-> on this so we can fix this bug with gcc 4.7.2 throwing internals in yo face
//        if (!_closed)
//            this->AsyncAccept<T>();
//    });
//}

//void WorldSocketMgr::OnSocketOpen(tcp::socket&& sock, uint32 threadIndex)
//{
//    // set some options here
//    if (_socketSystemSendBufferSize >= 0)
//    {
//        boost::system::error_code err;
//        sock.set_option(boost::asio::socket_base::send_buffer_size(_socketSystemSendBufferSize), err);
//        if (err && err != boost::system::errc::not_supported)
//        {
//            TC_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = %s", err.message().c_str());
//            return;
//        }
//    }
//
//    // Set TCP_NODELAY.
//    if (_tcpNoDelay)
//    {
//        boost::system::error_code err;
//        sock.set_option(boost::asio::ip::tcp::no_delay(true), err);
//        if (err)
//        {
//            TC_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = %s", err.message().c_str());
//            return;
//        }
//    }
//
//    //sock->m_OutBufferSize = static_cast<size_t> (m_SockOutUBuff);
//
//    BaseSocketMgr::OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
//}
//
//virtual void OnSocketOpen(tcp::socket&& sock, uint32 threadIndex)
//{
//    try
//    {
//        std::shared_ptr<SocketType> newSocket = std::make_shared<SocketType>(std::move(sock));
//        newSocket->Start();
//
//        _threads[threadIndex].AddSocket(newSocket);
//    }
//    catch (boost::system::system_error const& err)
//    {
//        TC_LOG_WARN("network", "Failed to retrieve client's remote address %s", err.what());
//    }
//}
