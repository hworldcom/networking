#include<iostream>
#include<string>

#include "../include/TCPClient.h"

using namespace std;


int main()
{
    TCPClient client("127.0.0.1", 54010);
    
    std::cout << "running"<< std::endl;

    client.run();
    
}

