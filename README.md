# Daytime-Server
* This system consists of two C programs, a server and client, that utilize TCP IPv4 sockets to communicate.
* dayserve.c is a server process that listens on an ephemeral TCP port for a connection (port 49999). When it recieves a connection, it logs the hostname of the client to stdout and writes the current date and time to the client (as text) via the socket connection. When the server recieves a connection, it forks a new child process to serve the client and goes back to waiting for more connections.
* daytime.c is a client program that takes one argument on the command line. That argument is the server's host name/IP address (in dotted decimal). It makes a TCP connection to the indicated host and dayserve's port number (or indicates an error if it cannot) and then writes the information recieved from dayserve to stdout.
* errno.h and string.h libraries are used to print any errors that occur, and the time() and ctime() routines are used to obtain and format the time of day.
## Screenshots
### Server
![Alt text](/screenshots/server.png?raw=true "Server")
### Client
![Alt text](/screenshots/client.png?raw=true "Client")