#include "tcpserver.hpp"

using namespace advcpp;


TcpServer::TcpServer(std::string a_ip ,int a_port, IHandler &a_handler , IDecoder &a_decoder)
:m_listeningSocket(new Socket(a_ip , a_port))
,m_handler(a_handler)
,m_decoder(a_decoder)
{
    FD_ZERO(&m_master);
    FD_ZERO(&m_temp);
    FD_SET(m_listeningSocket->GetSocket(), &m_master);
    m_listeningSocket->Reuse();
    m_listeningSocket->Bind();
    m_listeningSocket->Listen(BACK_LOG);
}

void TcpServer::Run()
{
    int activity;
    while(1)
    {
        m_temp = m_master;
        activity = select(1024, &m_temp, NULL, NULL, NULL);
        if(activity < 0)
        {
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
        }
        if (FD_ISSET(m_listeningSocket->GetSocket(), &m_temp) > 0)
        {
            CheckNewClients();
            if (activity == 1)
            {
                continue;
            }
            activity--;
        }
        CheckCurClients(activity);
    }
}

void TcpServer::Shutdown()
{
    m_listeningSocket->Close();
}

void TcpServer::CheckNewClients()
{
    ISocket* clientSocket = new Socket;
    clientSocket = m_listeningSocket->Accept();
    FD_SET(clientSocket->GetSocket() , &m_master);
    m_socketList.push_back(clientSocket);
}


void TcpServer::CheckCurClients(int a_activity)
{
    if(m_socketList.empty())
    {
        return;
    }
    char buffer[BUFFER_SIZE];
    std::list<ISocket*>::iterator it = m_socketList.begin();
    int readBytes;
    while (it != m_socketList.end() && a_activity > 0)
    {
        int clientSocket = (*it)->GetSocket();
        if (FD_ISSET(clientSocket, &m_temp) == 0)
        {
            ++it;
            continue;
        }
        readBytes = (*it)->Receive(buffer , BUFFER_SIZE);
        if(readBytes == 0)
        {
            m_socketList.erase(it++);
            FD_CLR(clientSocket, &m_master);
            close(clientSocket);
            delete *it;
        }
        else
        {
            std::string msg(buffer);
            m_handler.Handle(msg , *it , m_decoder);
            ++it;
        }
        a_activity--;
        ++it;
    }
}
