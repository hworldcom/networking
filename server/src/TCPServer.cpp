#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#include "../include/TCPServer.h"

TCPServer::TCPServer(std::string ipAddress, int port, MessageReceivedHandler handler)
    : m_ipAddress(ipAddress), m_port(port), messageReceived(handler)
    {

    }

TCPServer::~TCPServer()
{
    cleanup();
}

// send a message to a specified client
void TCPServer::send(int clientSocket, std::string msg)
{
    ::send(clientSocket, msg.c_str(), msg.size(), 0);
}

// review if needed
//int TCPServer::init()
//{
//    int listening = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -- IPV4
//
//    return listening;
//
//}

void TCPServer::run()
{

    char buf[MAX_BUFFER_SIZE];

    while(true)
    {
        std::cout << "listening"<< std::endl;
        //  create a listening socket
        int listening = CreateSocket();
        if (listening<0)
        {
            break;
        }
        int client = WaitForConnection(listening);

        if (!(client < 0))
        {
            std::cout << "client connected"<< std::endl;

            close(listening);

            int bytesRecv = 0;
            do
            {
                memset(buf,0,MAX_BUFFER_SIZE);
                bytesRecv = recv(client, buf, MAX_BUFFER_SIZE, 0);

                if (messageReceived != NULL)
                {
                    std::cout << "message received" << std::endl;
                    std::cout << buf << std::endl;

                    messageReceived(this, client, std::string(buf,0,bytesRecv));
                }

            }while(bytesRecv>0);

            close(client);
        }
        // wait for connection
        // loop received/ sent
    }
}

void TCPServer::cleanup()
{

}

int TCPServer::CreateSocket()
{
    //init();

    int listening = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -- IPV4
    if (listening == -1)
    {
        std::cerr << "Cant create a socket!";
        return -1;
    }


    sockaddr_in hint; // barebone server
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port); // little endian big endian processor
    inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr); // pointer to string to a number
    
    if (::bind(listening, (sockaddr*) &hint, sizeof(hint)) < 0) // for linux add AF_INET
    {
        std::cerr << "cant bind to IP/port";
        return -2;
    }

    if (listen(listening, SOMAXCONN)==-1)
    {
        std::cerr << "cant listen";
        return -3;
    }

    return listening;

}

int TCPServer::WaitForConnection(int listening) {
    
    // accept a call
    sockaddr_in client;
    //socklen_t 
    socklen_t clientSize = sizeof(client);


    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if (clientSocket==-1)
    {
        std::cerr << "Problem with client connecting!";
        return -4;
    }
    close(listening);
    return clientSocket;
}




