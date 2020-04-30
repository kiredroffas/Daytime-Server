/*  Erik Safford
 *  IPv4 Localhost Server Client Socket Connection
 *  Spring 2019
 *
 *  - server gets client name, client gets date/time from server  */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MY_PORT_NUMBER 49999

int main(int argc, char **argv) {
	//--------------------Make A Socket-----------------------------------------------------------------
	int listenfd;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //Create socket using IPv4 internet protocol (TCP)
		perror("socket");
		exit(1);
	}
	printf("Server socket created\n");

	//-------------------Bind The Socket To A Port------------------------------------------------------

	struct sockaddr_in servAddr;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(MY_PORT_NUMBER); //Port 49999
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(listenfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) { //Bind socket to a port
		perror("bind");
		exit(1);
	}
	printf("Socket bound to port\n");

	//--------------------Listen And Accept Connections-------------------------------------------------
	if ((listen(listenfd, 1)) < 0) { //Sets up a connection queue one level deep
		perror("listen");
		exit(1);
	}
	printf("Listening for connections...\n\n");

	//--------------------Accept Connections------------------------------------------------------------

	int connectfd, status;
	int length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	while (1) {
		//Blocks until a new connection is established by a client
		if ((connectfd = accept(listenfd, (struct sockaddr *)&clientAddr, &length)) < 0) {
			perror("accept");
			exit(1);
		}
		printf("Client connection found!\n");

		if (fork()) {			     //Parent process closes connection, and waits for child process to finish
			close(connectfd);		 //Close client file descriptor
			waitpid(-1, &status, 0); //-1 makes waitpid wait for any child processes to exit, prevents zombies
		}
		//--------------------Write to Client--------------------------------------------------------------------------
		else { //Child process gets name of connection, then writes date/time to client through socket connection
			struct hostent *hostEntry;
			char *hostName;
			//Convert a numerical Internet address to a host name
			if ((hostEntry = gethostbyaddr(&(clientAddr.sin_addr), sizeof(struct in_addr), AF_INET)) == NULL) {
				herror("gethostbyaddr");
				exit(1);
			}
			hostName = hostEntry->h_name;
			printf("Connected to: %s\n", hostName); //Print client name

			char buffer[100]; //Read in the date/time into buffer
			time_t ltime;
			time(&ltime);
			strcpy(buffer, ctime(&ltime));
			int len = strlen(buffer);
			buffer[len] = '\0';

			if ((write(connectfd, buffer, 100)) <= 0) { //Write buffer to client
				perror("write");
				exit(1);
			}
			printf("Wrote time and date to client\n");
			printf("%s exiting...\n\n", hostName);
			close(connectfd); //Close client file descriptor
			exit(0);		  //Exit child process
		}
	}
}
