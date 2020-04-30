#Compile both server and client executables
all: server client

server: dayserve.c
	gcc -o server dayserve.c

client: daytime.c
	gcc -o client daytime.c

#Remove object and temp files
clean :
	rm server client