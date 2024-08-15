#ifndef MV_HMI_SERVER_TEST_H
#define MV_HMI_SERVER_TEST_H
#include "tcp_server.h"

class CMvHmiServerTest:public TCPServer{
public:
    CMvHmiServerTest(unsigned short port);

protected:
    void handle_client(int client_sock) override;
};













#endif