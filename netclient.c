#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

double reverseDouble(double value){
    
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
        //reverse(sizeof(double), &src);
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
    int sockfd;
    socklen_t len;
    struct sockaddr_in address;
    int result, out = 0, request_id = 0;
    ;
    int serverPort = 9734;
    request rq;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(serverPort);
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("oops: netclient");
        exit(1);
    }

    do
    {
        int choice;

        printf("Type 1 to get square root \n");
        printf("Type 2 to get current date and time \n");
        printf("Type something else to exit \n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            double value;
            printf("Give number to make square root: ");
            scanf("%lf", &value);
            rq.byteOp0 = 0;
            rq.byteOp1 = 0;
            rq.byteOp2 = 0;
            rq.byteOp3 = 1;
            rq.value.square_root = check_endian(value);
            write(sockfd, &rq, sizeof(request));
            read(sockfd, &rq, sizeof(request));
            if (rq.byteOp0 == 1)
            {
                printf("Number extracted: %lf\n\n", check_endian(rq.value.square_root));
            }
            else
            {
                printf("Something went wrong");
            }
        }
        else if (choice == 2)
        {
            rq.byteOp0 = 0;
            rq.byteOp1 = 0;
            rq.byteOp2 = 0;
            rq.byteOp3 = 2;
            write(sockfd, &rq, sizeof(request));
            read(sockfd, &rq, sizeof(request));
            if (rq.byteOp0 == 1)
            {
                printf("Current date and time: %s\n", rq.value.date_str);
            }
            else
            {
                printf("Something went wrong");
            }
        }
        else
        {
            printf("\nSERVER STOP\n");
            out = 1;
        }
    } while (out == 0);

    close(sockfd);
}
