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
    //int server_sockfd;
	
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
        double num;

		/* getting system time */
		time_t rawtime;
  		struct tm * timeinfo;
  		time ( &rawtime );
  		timeinfo = localtime ( &rawtime );
		char *godzina2 = asctime(timeinfo);

        int dlugosc = strlen(godzina2);
        //printf("dlugosc to: %d \n", dlugosc);

        char godzina[dlugosc];
        strcpy(godzina,godzina2);
        
		//char g[10]="CYPIS";
		//printf("dlugosc tego chara to: %d", strlen(godzina));
		//char g[33]="poghjfjhfhfd";
        printf ("server waiting\n");
        printf ( "Current local time and date: %s", godzina );

        /*  Accept connection.  */

        client_len = sizeof (client_address);
        client_sockfd = accept (server_sockfd,
                (struct sockaddr *) &client_address,
                &client_len);

        /*  We can now read/write to the client on client_sockfd.
            The five second delay is just for this demonstration.  */

        read (client_sockfd, &num, sizeof(double));
        //read (client_sockfd, &godzina, 1);
        sleep (5);
		//godzina = asctime (timeinfo);
		//num=num*2.0;
        double num22 = sqrt(num);
		num = num22;
		
        write (client_sockfd, &num, sizeof(double));
        write (client_sockfd, &dlugosc, sizeof(int));
        write (client_sockfd, &godzina, dlugosc);
        close (client_sockfd);
    }
}