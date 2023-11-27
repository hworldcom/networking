#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
#include <fcntl.h>


#include "../include/TCPServer.h"


#define MAX_EVENTS 10



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

void TCPServer::run()
{
    int ret;

    struct epoll_event ev, events[MAX_EVENTS];
    int socket_fd, client_fd, epoll_fd;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -- IPV4
    if (socket_fd == -1)
    {
        std::cerr << "Cant create a socket!";
        exit(EXIT_FAILURE);
    }

    //socklen_t addrlen = sizeof(struct sockaddr);
    sockaddr_in addr; // barebone server
    socklen_t size_addr = sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port); // little endian big endian processor
    inet_pton(AF_INET, m_ipAddress.c_str(), &addr.sin_addr); // pointer to string to a number
    
    if (::bind(socket_fd, (sockaddr*) &addr, sizeof(addr)) < 0) // for linux add AF_INET
    {
        std::cerr << "cant bind to IP/port";
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, SOMAXCONN) < 0)
    {
        std::cerr << "cant listen";
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl: socket_fd");
        exit(EXIT_FAILURE);
    }

    char buf[1024];

    for (;;) {

        int number_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        std::cout << "here" << std::endl;

        if (number_fd == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < number_fd; ++n) {

            if (events[n].data.fd == socket_fd) {



                //struct sockaddr_in client_addr;
                //socklen_t size_client_addr = sizeof(clientAddress);

                client_fd = accept(socket_fd, (struct sockaddr *) &addr, &size_addr);
                if (client_fd == -1) {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    exit(EXIT_FAILURE);
                }
                       
                setnonblocking(client_fd);

                // add client socket to epoll
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                if (ret == -1) {
                    std::cerr << "Failed to add client socket to epoll instance." << std::endl;
                    close(client_fd);
                    exit(EXIT_FAILURE);
                }

            } else {
                //do_use_fd(events[n].data.fd);
                client_fd = events[n].data.fd;

                int buflen = read(client_fd, buf, sizeof(buf));
                if (buflen == -1)
                {
                    //error
                }

                if (buflen == 0)
                {
                    // close
                    close(client_fd);
                    if (client_fd == -1) { 
                        std::cerr << "close" << std::endl;
                        exit(1); 
                    }
                    // epoll_ctl
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, &events[n]);
                    if (ret == -1) {
                        std::cerr << "epoll_ctl" << std::endl;
                         exit(1); 
                    }
                }
                else 
                {
                    buf[buflen] = '\0';
                    //std::string msgPrefix = "some-prefix-to-prevent-arbitrary-connection";
                    //std::string msg = buf;
                    // do something with data
                    std::cout << "sending message back: " << std::endl;
                    std::string str_test(buf);
                    std::cout << str_test << "\r\n";

                    write(client_fd, buf, sizeof(buf));
                }

            }
        }
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

bool TCPServer::setnonblocking(int socketfd)
{
    int flags = fcntl(socketfd, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "[E] fcntl failed (F_GETFL)\n";
        return false;
    }

    flags |= O_NONBLOCK;
    int s = fcntl(socketfd, F_SETFL, flags);
    if (s == -1)
    {
        std::cerr << "[E] fcntl failed (F_SETFL)\n";
        return false;
    }

    return true;
}


