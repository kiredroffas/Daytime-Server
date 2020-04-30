/*  Erik Safford
 *  IPv4 Localhost Server Client Socket Connection
 *  Spring 2019
 *
 *  - server gets client name, client gets date/time from server  
 *  - reads hostName of server to join through argv[1], i.e. localhost or 127.0.0.1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MY_PORT_NUMBER 49999

//----------------Make a connection from client-----------------------------------------------
int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Invalid connection format, format is: <executable> <hostname/address>\n");
		exit(1);
	}
	
	int socketfd;
	
	if( (socketfd = socket( AF_INET, SOCK_STREAM, 0)) < 0) {  //Make socket using TCP protocol
		perror("socket");
		exit(1);
	}

//----------------Set up the address of the server--------------------------------------------

	struct sockaddr_in servAddr;
	struct hostent *hostEntry;
	struct in_addr **pptr;

	memset(&servAddr, 0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(MY_PORT_NUMBER);

	if( (hostEntry = gethostbyname(argv[1])) == NULL ) {
		herror("gethostbyname");
		exit(1);
	}

	/* Structure pointer magic */
	pptr = (struct in_addr**) hostEntry->h_addr_list;
	memcpy(&servAddr.sin_addr, *pptr,sizeof(struct in_addr));

//----------------Connect and Read From the Server--------------------------------------------------
	if( (connect(socketfd, (struct sockaddr *) &servAddr,sizeof(servAddr))) < 0 ) {
		perror("connect");
		exit(1);
	}
	
	//Read date/time from server
	char buffer[100];
	int byteRead = read(socketfd, buffer, 100);
	if(byteRead <= 0) {
		perror("read");
		exit(1);
	}
	buffer[byteRead] = '\0';
	printf("Current date/time from server: %s\n", buffer);
	close(socketfd);
	exit(0);
}
