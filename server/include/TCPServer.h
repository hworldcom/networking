#pragma once

#include<string>

#define MAX_BUFFER_SIZE (49152)

// forward declaration of class
class TCPServer;

//order:
// initialize socket
// create socket
// wait for connection
// receive loop
//      send back message
// cleanup

// callback to data received
// message handler
typedef void(*MessageReceivedHandler)(TCPServer* listener, int socketId, std::string msg);

class TCPServer 
{

public:

    TCPServer(std::string ipAddress, int port, MessageReceivedHandler handler);
    
    ~TCPServer();

    // initialize socket
    //int init();

    // wait for connection
    // receive loop

    // the main processing loop
    void run();

    // send back message to a specific client
    void send(int clientSocket, std::string msg);

    // cleanup
    void cleanup();




private:
    // create socket
    int CreateSocket();

    // wait for a connection
    int WaitForConnection(int listening);
    bool setnonblocking(int socketfd);

    std::string m_ipAddress;
    int m_port;
    MessageReceivedHandler messageReceived;

};

