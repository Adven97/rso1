#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

int main ()
{
    struct sockaddr_in address;
    int dlugosc;

    double num;
    printf("Podaj liczbe do spierwiastkowania \n");
    scanf("%lf", &num);

    /*  Create a socket for the client.  */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr ("127.0.0.1");
    address.sin_port = htons (9734);
    socklen_t len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */
    int result = connect (sockfd, (struct sockaddr *) &address, len);
   

    if (result == -1)
    {
        perror ("oops: netclient");
        exit (1);
    }

    /*  We can now read/write via sockfd.  */
    write (sockfd, &num, sizeof(double));
    read (sockfd, &num, sizeof(double));
    read (sockfd, &dlugosc, sizeof(int));
    
    char godzina[dlugosc];
    //char *godzina = (char *)malloc(dlugosc * sizeof(char));

    read (sockfd, &godzina, dlugosc);

    printf ("Liczba spierwiastkowana = %.4f\n", num);
	printf ( "Obecny czas i data:\n" );
	printf ("%s\n",godzina);
    
    close (sockfd);
    exit (0);
}