#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

double reverseDouble(double value)
{
    double result;
    char *old_v = (char *)&value;
    char *new_v = (char *)&result;
    for (size_t i = 0; i < sizeof(value); i++)
    {
        new_v[i] = old_v[sizeof(value) - i - 1];
    }
    return result;
}

double check_endian(double src)
{
    if (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
       // printf("there is little endian");
        src = reverseDouble(src);
    return src;
}

typedef union {
    double square_root;
    char date_str[16];
} user_data;

typedef struct
{
    uint8_t byteOp0;
    uint8_t byteOp1;
    uint8_t byteOp2;
    uint8_t byteOp3;
    int RQID : 4;
    user_data value;
} request;

int main()
{
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    listen(server_sockfd, 5);

    signal(SIGCHLD, SIG_IGN);
    while (1)
    {

        printf("server waiting\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        if (fork() == 0)
        {
            request rq;
            while (1)
            {
                read(client_sockfd, &rq, sizeof(request));
                if (rq.byteOp3 == 1)
                {
                    rq.value.square_root = sqrt(check_endian(rq.value.square_root));
                    rq.value.square_root = check_endian(rq.value.square_root);
                    rq.byteOp0 = 1;
                }
                else if (rq.byteOp3 == 2)
                {
                    time_t now = time(0);
                    strcpy(rq.value.date_str, ctime(&now));
                    rq.byteOp0 = 1;
                }
 
                send(client_sockfd, &rq, sizeof(request), 0);
            } 
        }
        close(client_sockfd);
    }

}
