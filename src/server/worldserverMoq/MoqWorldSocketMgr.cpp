#include "MoqWorldSocketMgr.h"
#include "AsyncAcceptor.h"
#include "Errors.h"
#include "NetworkThread.h"
#include "WorldSocket.h"
#include "Config.h"
#include "ScriptMgr.h"
#include "WorldSocketMgr.h"

MoqWorldSocketMgr& MoqWorldSocketMgr::Instance()
{
    static MoqWorldSocketMgr instance;
    return instance;
}

void MoqWorldSocketMgr::OnSocketOpen(tcp::socket&& sock, uint32 threadIndex) {
    // set some options here
    int32 socketSystemSendBufferSize = sConfigMgr->GetIntDefault("Network.OutKBuff", -1);
    bool tcpNoDelay = false;// sConfigMgr->GetBoolDefault("Network.TcpNodelay", true);
    if (socketSystemSendBufferSize >= 0)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::socket_base::send_buffer_size(socketSystemSendBufferSize), err);
        if (err && err != boost::system::errc::not_supported)
        {
            TC_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = %s", err.message().c_str());
            return;
        }
    }

    // Set TCP_NODELAY.
    if (tcpNoDelay)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::ip::tcp::no_delay(true), err);
        if (err)
        {
            TC_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = %s", err.message().c_str());
            return;
        }
    }

    BaseSocketMgr::OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
    /*std::shared_ptr<WorldSocket> _socket = std::make_shared<WorldSocket>(sock);
    _socket->Start();*/

    /*try
    {
        std::shared_ptr<SocketType> newSocket = std::make_shared<SocketType>(std::move(sock));
        newSocket->Start();

        _threads[threadIndex].AddSocket(newSocket);
    }
    catch (boost::system::system_error const& err)
    {
        TC_LOG_WARN("network", "Failed to retrieve client's remote address %s", err.what());
    }*/
};

std::shared_ptr<WorldSocket> MoqWorldSocketMgr::GetSocket() {
    return _socket;
};
