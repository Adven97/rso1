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

int main(){

    // create a socekt [int socket(int domain, int type, int protocol)];
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }
    // bind the socket to a IP/PORT
    sockaddr_in server_address; // it is a bufer that will be used in comunication between client and server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);                   //htons converts the unsigned short integer hostshort from host byte order to network byte order.
    socklen_t server_size = sizeof(server_address);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); //it's converts a number to an array of integers

    int binded = bind(server_sock, (sockaddr *)&server_address, server_size);
    if (binded == -1){
        cerr << "Sorry, can't bind";
        return -2;
    }

    int listened = listen(server_sock, SOMAXCONN); // mark the socket for listening in
    if (listened == -1){
        cerr << "Sorry, can't listen";
        return -3;
    }

    // accept 
    sockaddr_in client;
    socklen_t client_size = sizeof(client);

    int client_soc = accept(server_sock, (sockaddr *)&client, &client_size);
    if (client_soc == -1){
        cerr << "Can't connect with client!";
        return -4;
    }
    close(server_sock);

    cout << "Server is up and ready!" << endl;

    // while receving display echo message
    char buffer[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    while (true)
    {
        // clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // wait for message
        int bytesRecv = recv(client_soc, buffer, BUFFER_SIZE, 0); // receives data from a connected socket or a bound connectionless socket.
         // receives data from a connected socket or a bound connectionless socket.
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
        
        string message_to_send;    

        if (buffer[0] == '0'){
            message_to_send = "Current date: " + date_time;
        }
        else if (buffer[0] == '1')
        {
            message_to_send = " Sorry, unknown input, can't make square root "; /// initial value to sends displays wrong input message

            int bytesRecv2 = recv(client_soc, buffer2, BUFFER_SIZE, 0);
            if (bytesRecv2 <= 0)
            {
                cerr << "Something went wrong" << endl;
                break;
            }

            double user_value;
            string user_input = "";
            bool is_dot = false;
            bool correct = true;
            
            //check if number in correct order
            for (int i = 0; i < bytesRecv2 - 1; i++)
            {
                if (buffer2[i] >= 48 && buffer2[i] <= 57)
                {
                    user_input += buffer2[i];
                }

                else if (buffer2[i] == '.' && !is_dot)
                {
                    user_input += buffer2[i];
                    is_dot = true;
                }

                else
                {
                    correct = false;
                    break;
                }
            }

            if (correct)
            {
                user_value = atof(user_input.c_str());
                cout << "USER VALUE = " << user_value << endl;

                user_value = sqrt(user_value);
                message_to_send = "Number extracted : "+to_string(user_value);
            }
        }
        else{
            message_to_send = "Sorry, wrong input, try again";
        }

        int message_len = message_to_send.length();
        memset(buffer, 0, BUFFER_SIZE);
        for (int x = 0; x < message_len; x++){
        
            buffer[x] = message_to_send[x];
        }
        //send rsponse
        send(client_soc, buffer, message_len + 1, 0);
    }
    // close socket
    close(client_soc);
    return 0;
}
