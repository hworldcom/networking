#include<iostream>
#include<string>

#include "../include/TCPServer.h"

using namespace std;

void Listener_MessageReceived(TCPServer* listener, int client, string msg);

int main()
{
    TCPServer server("127.0.0.1", 54010, Listener_MessageReceived);


    server.run();
    
}

void Listener_MessageReceived(TCPServer* listener, int client, string msg) {
    // extracts handing of networking to this layer
    // any message received can be manipulated here
    // no specialized version over TCP class


}