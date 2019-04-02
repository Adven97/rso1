#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>

int main ()
{
	
    socklen_t server_len;
	socklen_t client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int server_sockfd = socket (AF_INET, SOCK_STREAM, 0);
	int client_sockfd;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl (INADDR_ANY);
    server_address.sin_port = htons (9734);
    server_len = sizeof (server_address);
    bind (server_sockfd, (struct sockaddr *) &server_address, server_len);
	
    /*  Create a connection queue and wait for clients.  */
    listen (server_sockfd, 5);

    while (1)
    {
        printf("server waiting\n");

        /*  Accept connection.  */

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,
                               (struct sockaddr *)&client_address,
                               &client_len);


        double num;

		/* getting system time */

		time_t rawtime;
  		struct tm * timeinfo;
  		time ( &rawtime );
  		timeinfo = localtime ( &rawtime );

        //time_t mytime = time(NULL);


        char *godzina2 = asctime(timeinfo);
        //godzina2[strlen(godzina2) - 1] = '\0';
        int dlugosc = strlen(godzina2);
        
        char godzina[dlugosc];
        strcpy(godzina,godzina2);
        godzina[dlugosc-1] ='\0';


        /*  We can now read/write to the client on client_sockfd.
            The five second delay is just for this demonstration.  */

        read (client_sockfd, &num, sizeof(double));
        sleep (5);

        double num_temp = sqrt(num);
		num = num_temp;
		
        write (client_sockfd, &num, sizeof(double));
        write (client_sockfd, &dlugosc, sizeof(int));
        write (client_sockfd, &godzina, dlugosc);
        close (client_sockfd);
    }
}