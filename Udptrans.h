#ifndef UDPTRANS_H
#define UDPTRANS_H

#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>
#include<pthread.h>

#define SERVER_PORT 8000

class CUDPTrans
{
public:
    CUDPTrans(bool IsMain);
    ~CUDPTrans();
    int Init();
    int SocketInit();
    int Connect();
    int ReConnect();
private:
    static void* ReceiveThread( void * );
    static void* SendThread(void *);
    void RunReceive();
    void RunSend();
    pthread_t m_ReceiveThread;
    pthread_t m_SendThread;
    bool m_bIsMainCap;
    struct sockaddr_in m_tagLocalAddr,m_tagRemoteAddr;
    int m_ServerSocket_fd;
};

#endif // UDPTRANS_H
