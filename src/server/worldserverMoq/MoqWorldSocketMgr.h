#include "WorldSocketMgr.h"
#include "SocketMgr.h"
#include "NetworkThread.h"
#include "Config.h"
#include "AsyncAcceptor.h"
#include "Errors.h"
#include "NetworkThread.h"
#include "WorldSocket.h"

class MoqWorldSocketMgr : public WorldSocketMgr
{
    typedef SocketMgr<WorldSocket> BaseSocketMgr;
public:
    static MoqWorldSocketMgr& Instance();
    void OnSocketOpen(tcp::socket&& sock, uint32 threadIndex);
    std::shared_ptr<WorldSocket> GetSocket();
protected:
    MoqWorldSocketMgr() : WorldSocketMgr()
    {

    }


private:
    std::shared_ptr<WorldSocket> _socket;
};
#define sMoqWorldSocketMgr MoqWorldSocketMgr::Instance()
