#include "AsyncAcceptor.h"
#include "IoContext.h"
#include "IpAddress.h"
#include "Log.h"
#include "WorldSocket.h"
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <atomic>

using boost::asio::ip::tcp;

#if BOOST_VERSION >= 106600
#define TRINITY_MAX_LISTEN_CONNECTIONS boost::asio::socket_base::max_listen_connections
#else
#define TRINITY_MAX_LISTEN_CONNECTIONS boost::asio::socket_base::max_connections
#endif

class MoqAsyncAcceptor : public AsyncAcceptor
{
public:
    MoqAsyncAcceptor(Trinity::Asio::IoContext& ioContext, std::string const& bindIp, uint16 port) :
        AsyncAcceptor(ioContext, bindIp, port),
        _acceptor(ioContext), _ioContext(ioContext), _socket(ioContext),
        _closed(false), _socketFactory(std::bind(&MoqAsyncAcceptor::DefeaultSocketFactory, this))
    {
        //std::pair<tcp::socket*, uint32> DefeaultSocketFactory() { return std::make_pair(&_socket, 0); }
        //std::function<std::pair<tcp::socket*, uint32>()> _socketFactory(std::bind(&AsyncAcceptor::DefeaultSocketFactory, this))
    }
    std::shared_ptr<WorldSocket> BuildWorldSocket();
private:
    std::pair<tcp::socket*, uint32> DefeaultSocketFactory() { return std::make_pair(&_socket, 0); }

    tcp::acceptor _acceptor;
    Trinity::Asio::IoContext& _ioContext;
    tcp::socket _socket;
    std::atomic<bool> _closed;
    std::function<std::pair<tcp::socket*, uint32>()> _socketFactory;
};
