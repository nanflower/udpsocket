#include "Udptrans.h"

CUDPTrans::CUDPTrans(bool IsMain)
{
    m_bIsMainCap = IsMain;
}

CUDPTrans::~CUDPTrans()
{
    close(m_ServerSocket_fd);
}

int CUDPTrans::Init()
{
    int res;
    res = SocketInit();
    if(-1 == res)
    {
        printf("Socket Init Failed.\n");
        return -1;
    }

    if(0 != pthread_create(&m_SendThread, NULL, SendThread, this))
    {
        printf("Create Send Thread Failed.\n");
        return -1;
    }

    if(0 != pthread_create(&m_ReceiveThread, NULL, ReceiveThread, this))
    {
        printf("Create Receive Thread Failed.\n");
        return -1;
    }

    return 0;
}

int CUDPTrans::SocketInit()
{
    if(m_bIsMainCap)
    {
        bzero(&m_tagLocalAddr, sizeof(m_tagLocalAddr));
        m_tagLocalAddr.sin_family = AF_INET;
        m_tagLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        m_tagLocalAddr.sin_port = htons(SERVER_PORT);
    }else
    {
        bzero(&m_tagLocalAddr, sizeof(m_tagLocalAddr));
        m_tagLocalAddr.sin_family = AF_INET;
        m_tagLocalAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        m_tagLocalAddr.sin_port = htons(SERVER_PORT);
    }

    /* 创建socket */
    m_ServerSocket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == m_ServerSocket_fd)
    {
        perror("Create Socket Failed\n");
        return -1;
    }

    /* 绑定套接口 */
    if(-1 == (bind(m_ServerSocket_fd,(struct sockaddr*)&m_tagLocalAddr,sizeof(m_tagLocalAddr))))
    {
        perror("Server Bind Failed:");
        return -1;
    }

    return 0;
}

void *CUDPTrans::SendThread(void * pArg)
{
    CUDPTrans* pTemp = (CUDPTrans*)pArg;
    if( pTemp )
        pTemp->RunSend();
    return (void*)NULL;
}

void CUDPTrans::RunSend()
{
    char smsg[] = {"This is a UDP test."};
    int ret=sendto(m_ServerSocket_fd, smsg, strlen(smsg), 0, (sockaddr*)&m_tagLocalAddr, sizeof(m_tagLocalAddr));
    if(ret<0)
    {
        printf("connect error\n");
    }
    else
    {
        printf("ok connect\n");
    }
}

void *CUDPTrans::ReceiveThread( void * pArg)
{
    CUDPTrans* pTemp = (CUDPTrans*) pArg;
    if( pTemp )
        pTemp->RunReceive();
    return (void*)NULL;
}

void CUDPTrans::RunReceive()
{
    socklen_t client_addr_length = sizeof(m_tagRemoteAddr);
    if(m_bIsMainCap)
    {
        printf("ok.\n");
        /* 接收数据 */
        char buffer[100];
        bzero(buffer, 100);
        if(recvfrom(m_ServerSocket_fd, buffer, 100,0,(struct sockaddr*)&m_tagRemoteAddr, &client_addr_length) == -1)
        {
            perror("Receive Data Failed:");
            exit(1);
        }
        printf("%s\n",buffer);
        printf("get out\n");
    }
}

