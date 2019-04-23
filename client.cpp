#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <string>

#define PORT 9734
#define BUFFER_SIZE 100

using namespace std;

int main(){
    // create socket
    int client_soc= socket(AF_INET, SOCK_STREAM, 0);
    if (client_soc== -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }

    sockaddr_in client_addr; // it is a bufer that will be used in comunication between client and server
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);                    //htons will transle it into bytes
    inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr); //it's converts a number to an array of integers //0.0.0.0 will give us any existing ip address
    
    // connect to the server on the socket
    int response = connect(client_soc, (sockaddr *)&client_addr, sizeof(sockaddr_in));
    if (response == -1)
    {
        cerr << "Can't connect with server";
        return -4;
    }

    char buffer[BUFFER_SIZE];
    string userInput;
    string inp2;
    while (true){

        cout << "Type 0 to get date"<<endl;
        cout << "Type 1 to get square root"<<endl;
        cin >> userInput;

        // send to server
        // we are adding one because we want to have one special place for 0 that will mean end of user buffer
        int response = send(client_soc, userInput.c_str(), userInput.size() + 1, 0); 
        if (response <= 0)
        {
            cerr << "Can't send, something went wrong" << endl;
            continue;
        }

        if (userInput == "1")
        {
            cout << "Give number to make square root: ";
            cin >> inp2;

            int response2 = send(client_soc, inp2.c_str(), inp2.size() + 1, 0);
            if (response2 <= 0)
            {
                cerr << "Can't send, something went wrong" << endl;
                continue;
            }
        }

        // clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // wait for response
        int bytesRecv = recv(client_soc, buffer, BUFFER_SIZE, 0);
        cout << "SERVER: " <<buffer << endl;

    } 

    close(client_soc);
    return 0;
}
