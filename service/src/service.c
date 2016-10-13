/*
 * service.c
 *Service program
 *  Created on: Oct, 2015
 *      Author: manisha agrawal
 */

#include "service.h"

//Get IP address of the directory from a file
int get_serverip(char *serv_ip)
{
	FILE *fp;
	char msg[SIZE];
	fp = fopen(DIR_IP_FILE,"r"); // read mode

	//If file doesnot return error
	if( fp == NULL )
		return 1;

	fgets ( msg, sizeof(msg), fp ) ;
	strcpy(serv_ip,msg);
	fclose(fp);
	return 0;
}

//get the timestamp
void format_time(char *output){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    sprintf(output, "%d-%d-%d %d-%d-%d",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

//start a new service for a client
int start_service()
{
	socklen_t client_addr_len;
	struct sockaddr_in client_addr, second_addr;
	char message[SIZE];
	int send_status,connect_status,rcv_count,flags,bind_status, listen_status;
	int second_socket, first_socket;
	char buf[SIZE];

	flags = 0;
	// create a socket
	second_socket = socket(AF_INET,SOCK_STREAM,0);
	if (second_socket<0) {
		perror("error opening the second socket:");
		exit(1);
	}

	// bind it to a port and advertise it
	second_addr.sin_family = AF_INET;
	second_addr.sin_port = MY_PORT1;
	second_addr.sin_addr.s_addr = INADDR_ANY;

	bind_status = bind(second_socket,(struct sockaddr *) & second_addr,sizeof(struct sockaddr_in));
	if (bind_status<0) {
		perror("error on binding second_status:");
		exit(1);
	}

	// inform the system how many unserved connections are allowed
	listen_status = listen(second_socket,MY_UNSERVED);
	if (listen_status<0) {
		perror("error on listen for fist socket:");
		exit(1);
	}
	client_addr_len = sizeof(client_addr);

	// wait and accept connections
	printf("Waiting for connections\n");

	while (1)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		first_socket = accept(second_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		if (first_socket<0) {
			perror("error on accept on first socket:");
	        exit(1);
	    }

	    //Connect from one service
	    printf( "I accepted a connection from %s\n", inet_ntoa(client_addr.sin_addr) );

	    //Receive the service request from devices
	    rcv_count=recv(first_socket,buf,SIZE,flags);
	    if(rcv_count<=0)
	    {
	    	perror("error connecting with the client!!");
	    	continue;
	    }

	    strcpy(buf, "Hello client!!");
	    send_status = send(first_socket,buf,strlen(buf)+2,flags);
	   }

	   return 0;
	}

