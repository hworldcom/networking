#pragma once

#include<string>


class TCPClient {
    public:
        TCPClient(std::string ipAddress, int port);
        ~TCPClient();
        void run();

        int send(int clientSocket, std::string message);
        void cleanup();

    private:
        std::string m_ipAddress;
        int m_port;

        int createSocket();

};