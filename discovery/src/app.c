/*
 * app.c
 *
 *  Created on: Oct 12, 2015
 *      Author: manisha
 *      Project:Discovery
 */
#include "discovery.h"

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

//Get what service you want to request from a file
int get_service(char *buf)
{
	FILE *fp;

	fp = fopen(SERVICE,"r");

	//Check if file  exist
	if( fp == NULL )
	{
		fp=fopen(SERVICE,"w");
		fputs(DEFAULT_SERVICE,fp);

		strcpy(buf, DEFAULT_SERVICE); //default service
	}
	else
	{
		if(fgets(buf, SIZE, fp) == NULL)
			strcpy(buf, DEFAULT_SERVICE); //default service
	}
	fclose(fp);
	return 0;
}

//Save the service details(IP addr and port #) obtained from the directory in a file
int save_service(char *buf, char *serv_ip, char *portno)
{
	FILE *fp;
	fp = fopen(SERVICE_INFO,"w+");

	fputs(buf,fp);
	fclose(fp);

	//get service IP address
	char *p = strchr(buf, ':');
	p++;
	while(*p != '\n')
	{
	    *serv_ip++ = *p++;
	}

	*serv_ip = '\0';

	//get portno
	while(*p++ != ':');
	while(*p != '\n')
	    *portno++ = *p++;
	*portno = '\0';

	return 0;
}

//Check if the user wants to invoke the service
int invoc()
{
	FILE *fp;

	fp = fopen(INVOC_FILE,"r");
	if(fp==NULL)
		return 1;

	remove(INVOC_FILE);
	return 0;

}

//Communicate with the service you requested for.
int invoke(char *service_ip, char *ip_addr, int portno)
{
	int first_socket, connect_status;
	struct sockaddr_in first_addr;
	int flags, rcv_count, send_status;
	char message[SIZE], buf[SIZE];

	flags =0;
	// create a socket
	first_socket = socket(AF_INET,SOCK_STREAM,0);
	if (first_socket<0) {
		return 1;
	}

	// connect to it;
	first_addr.sin_family = AF_INET;
	first_addr.sin_port = portno;
	first_addr.sin_addr.s_addr = inet_addr(service_ip);
	connect_status = connect(first_socket,(struct sockaddr *) & first_addr,sizeof(struct sockaddr_in));
	if (connect_status<0) {
		close(first_socket);
		return 1;
	}

   sprintf(message,"Requesting service from %s",ip_addr);
   send_status = send(first_socket,message,strlen(message)+2,flags);

   if (send_status<0)
   {
		close(first_socket);
		return 1;
   }

   buf[0]='\0';
   //Receive the service request from devices
   rcv_count=recv(first_socket,buf,SIZE,flags);
   if(rcv_count<=0)
   {
	   close(first_socket);
		return 1;
   }
   printf("Server message:<%s>\n",buf);

   close(first_socket);
   return 0;
}
