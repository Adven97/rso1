#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#define PORT 9734
#define BUFFER_SIZE 100
// AF_INET - Ipv4 format
//SOCK_STREAM - TCP
// 0 - IP
// SOMAXCONN - max number of connections
using namespace std;

int main()
{
    // create a socekt [int socket(int domain, int type, int protocol)];
    int listenningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenningSocket == -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }
    // bind the socket to a IP/PORT
    sockaddr_in server_address; // it is a bufer that will be used in comunication between client and server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);                   //htons converts the unsigned short integer hostshort from host byte order to network byte order.
    socklen_t server_size = sizeof(server_address);
    inet_pton(AF_INET, "0.0.0.0", &server_address.sin_addr); //it's converts a number to an array of integers //0.0.0.0 will give us any existing ip address

    int binded = bind(listenningSocket, (sockaddr *)&server_address, server_size);
    if (binded == -1)
    { 
        cerr << "Sorry, can't bind";
        return -2;
    }

    int listened = listen(listenningSocket, SOMAXCONN); // mark the socket for listening in
    if (listened == -1)
    {
        cerr << "Sorry, can't listen!";
        return -3;
    }
    
    // accept 
    sockaddr_in client;
    socklen_t client_size = sizeof(client);

    int clientSocket = accept(listenningSocket, (sockaddr *)&client, &client_size);
    if (clientSocket == -1)
    {
        cerr << "Can't connect with client!";
        return -4;
    }
    close(listenningSocket);

    cout << "Server is up and ready!" << endl;

    // while receving display echo message
    char buffer[BUFFER_SIZE];
    while (true)
    {
        // clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // wait for message
        int bytesRecv = recv(clientSocket, buffer, BUFFER_SIZE, 0); // receives data from a connected socket or a bound connectionless socket.

        if (bytesRecv <= 0)
        {
            cerr << "Something went wrong" << endl;
            break;
        }

        // get time of request
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream s;
        s << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        string date_time= s.str();


        // check if number
        string user_input = "";
        bool is_dot = false;
        bool correct = true;

        double user_value = 0;
        string messageToSend = date_time + " Sorry, unknown input";

        /// check if user typed number and if big endian
        for (int i = 0; i < bytesRecv - 1; i++)
        {
            if (buffer[i] >= 48 && buffer[i] <= 57)
            {
                user_input += buffer[i];
            }
            else if (buffer[i] == '.' && !is_dot)
            {
                user_input += buffer[i];
                is_dot=true;
            }
            else
            {
                correct = false;
                break;
            }
        }

        if(correct){
        
            user_value = atof(user_input.c_str());
            cout << "USER VALUE = " << user_value << endl;

            user_value = sqrt(user_value);
            messageToSend = date_time + "  " + to_string(user_value);
        }

        memset(buffer, 0, BUFFER_SIZE);
        for (int j = 0; j < messageToSend.length(); j++)
        {
            buffer[j] = messageToSend[j];
        }
        //send rsponse
        send(clientSocket, buffer, messageToSend.length() + 1, 0);
    }
    // close socket
    close(clientSocket);
    return 0;
}
