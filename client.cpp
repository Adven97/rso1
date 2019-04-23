#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <string>

#define PORT 9734
#define BUFFER_SIZE 1000

using namespace std;

int main(){
    // create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }

    sockaddr_in client_addr; // it is a bufer that will be used in comunication between client and server
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);                    //htons will transle it into bytes
    inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr); //it's converts a number to an array of integers //0.0.0.0 will give us any existing ip address
    // connect to the server on the socket
    int serverRes = connect(clientSocket, (sockaddr *)&client_addr, sizeof(sockaddr_in));
    if (serverRes == -1)
    {
        cerr << "Can't connect with server";
        return -4;
    }

    char buffer[BUFFER_SIZE];
    string userInput;
    string inp2;
    while (true){

        cout << "Type 0 to get date: "<<endl;
        cout << "Type 1 to get square root: "<<endl;
        cin >> userInput;

        // send to server
        int response = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0); // we are adding one because we want to have one special place for 0 that will mean end of user buffer
        if (response == -1){
            cerr << "Culdn't send message" << endl;
            continue;
        }
        if (response == 0){
            cerr << "Server disconnected" << endl;
            break;
        }

        if (userInput == "1")
        {
            cout << "Give number to make square root: ";
            cin >> inp2;

            int response2 = send(clientSocket, inp2.c_str(), inp2.size() + 1, 0); // we are adding one because we want to have one special place for 0 that will mean end of user buffer
            if (response2 == -1)
            {
                cerr << "Culdn't send message" << endl;
                continue;
            }
            if (response2 == 0)
            {
                cerr << "Server disconnected" << endl;
                break;
            }
            
        }

        // clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // wait for response
        int bytesRecv = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        cout << "SERVER: " <<buffer << endl;

    } 

    close(clientSocket);
    return 0;
}
