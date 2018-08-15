
#include<stdio.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdarg.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>

#include<rpc/rpc.h>
#include "rfile.h"
#define RMACHINE "localhost"
CLIENT* handle;


/*
 * Function for connecting to server
 * Arguments:
 * server - hostname to which the client want to be connected
 * transport - Transport layer protocol (tcp/udp)
 * prtnumber - server port
 */
//int connectServer(char *server, char *transport, char *portnumber);

#define FILEPATH "/home/karthika/Desktop/"				//Path where file to be stored
#define BUFSIZE 1024									//Buffer size for reading

int main(int argc, char **argv)
{
	char *server, *portnumber;
	char* transport = "tcp";
	char buf[BUFSIZE+1] = {0};
	char file[120] = {0};
	char fileName[128]= {0};
	int reading;
	int counter_read = 0;

	printf("%s \n", "client program started");

	if(argc == 4)
	{
		server = argv[1];
		portnumber = argv[2];
		strcpy(fileName, argv[3]);
		printf("file is %s \n", fileName);
	}
	else if(argc == 3)
	{
		server = argv[1];
		portnumber = argv[2];
		printf("please enter the file name to be transferred \n");
		exit(1);
	}
	else{
		printf("please enter the server details \n "
				"Server name, portnumber, file name to be transferred \n");
		exit(1);
	}


	handle = clnt_create(RMACHINE, RFILETRANSFER, RFILEVER, "tcp");

	if(handle == 0)
	{
		printf("Could not contact the remote program\n");
		exit(1);
	}


	//int sock = connectServer(server, transport, portnumber);

	//Writing to the server
	if(write(sock, fileName, strlen(fileName) ) < 0)
	{
		printf("error in write \n");
		exit(1);
	}
	else
	{
		printf("Request for file with name %s sent to server \n", fileName);
	}

	//Appending file name
	strcat(file, FILEPATH);
	strcat(file, fileName);

	printf("path where the filename is written is  %s \n", file);


	//Creating the file to be stored

	FILE *fptr;
	fptr = fopen(file, "w");

	if(!fptr)
	{
		printf("Error in creating / opening file. \n");
		exit(1);
	}
	else
	{
		printf("%s\n", "Opened the file at this point.");
	}

	printf("%s\n", "trying to read from server and write in the specified path..." );

	//Reading response from the server
	 reading = read(sock, buf, BUFSIZE+1);

	 if(reading < 0)
	 {
	 	printf("%s\n", "not able to read from server");
		exit(1);
	 }
	 else
	 {
	 	printf("bytes_received: %d\n", reading );
	 	printf("%s\n", buf);

	 }
	 while( reading > 0)
	 {
		counter_read += reading;
	 	//printf("%s\n", buf);
	 	printf("%s\n", "writing." );
	 	fwrite(buf,1,reading,fptr);
	 	printf("write done. \n");
	 	reading = read(sock, buf, BUFSIZE+1);
	 	printf("bytes_received: %d\n", reading );
	 }

	printf("total number of bytes read is %d \n", counter_read);
	printf("Closing file and socket.\n");
	fclose(fptr);
	close(sock);
}


int connectServer(char *server, char *transport, char *portnumber){

	int counter_num = 0;
	int sock,type, port;
	socklen_t len;

	struct sockaddr_in sin;		//Object of struct sockaddr_in
	struct hostent *phe;		//Pointer to hostent
	struct protoent *ppe;		//Pointer to protoent

	//Setting values in sin
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	port = atoi(portnumber);
	sin.sin_port = htons((uint16_t)port);

	printf("server is %s \n", server);

	//Mapping hostname to ip address
	phe = gethostbyname(server);
	if(!phe)
	{
		printf("error in mapping hostname to ip \n");
		exit(1);
	}
	else
	{
		memcpy(&sin.sin_addr.s_addr, phe->h_addr, phe->h_length);
		printf("hostname mapping successful \n");

	}

	//Mapping tcp to protocol number
	ppe = getprotobyname("tcp");
	if(!ppe)
	{
		printf("error in mapping protocol number to ip \n");
		exit(1);
	}
	else
	{
		printf("protocol number is %d\n", ppe -> p_proto);
	}


	//Creating socket
	sock = socket(PF_INET, SOCK_STREAM, ppe->p_proto);

	if(sock <0)
	{
		printf("error in client socket creation \n");
		exit(1);
	}
	else
	{
		printf("client sock created sucessfully # %d \n",sock);
	}

	//Connecting to the server
	if(connect(sock,(struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		printf("error in connecting to server \n");
		exit(1);
	}
	else
	{
		printf("connected to server \n");
	}

	return sock;

}


