#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "../include/TCPClient.h"


TCPClient::TCPClient(std::string ipAddress, int port)
    : m_ipAddress(ipAddress), m_port(port)
{

}

TCPClient::~TCPClient()
{
    cleanup();
}

void TCPClient::run()
{
    int connection = createSocket();
    // while loop

    char buf[1024];
    std::string userInput;

    do {
        // enter lines of text
        std::cout << "> ";
        getline(std::cin, userInput);

        // send to server
        server_fd = ::send(connection, userInput.c_str(), userInput.size(), 0);

        if (server_fd == -1)
        {
            std::cerr << "could not send to server! \n\n";
            continue;
        }

        // wait for response
        memset(buf, 0, 1024);
        int bytesReceived = recv(connection, buf, 1024, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "there was an error getting response from the server \r\n";
        }
        else 
        {
            std::cout << "Message back received: " << "\r\n";
            std::string message_back(buf);
            std::cout << "SERVER: " << message_back << "\r\n";
        }
                
    } while (true);

    close(connection);

}

int TCPClient::createSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        std::cerr << "Cant create a socket!";
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

    // connect to the server on the socket

    int connectRes = connect(sock, (sockaddr*) &hint, sizeof(hint));

    if (connectRes == -1)
    {
        std::cerr << "Problem with server connection!";
        return -2;
    }

    return sock;
}

int TCPClient::send(std::string msg)
{
    if(sendRes<0)
    {
        std::cerr<< "cannot send a message: no connection to server" << std::endl;
    }
    int sendRes = ::send(sendRes, msg.c_str(), msg.size(), 0);

    return sendRes;
}


void TCPClient::cleanup()
{

}


