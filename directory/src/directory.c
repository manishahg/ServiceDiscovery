/*
 ============================================================================
 Name        : directory.c
 Author      : manisha agrawal
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */
#include "dir.h"

//Global variables
char serv_id[SIZE];
int listen_status, bind_status, send_status;
int first_socket, second_socket;
char SERVICE_ADDR_STR[100];
int scanner_count, proj_count;

int main(int argc, char * argv[])
{
	socklen_t client_addr_len;
	struct sockaddr_in first_addr,client_addr;
    struct hostent *entry;
    char hostname[50], dis_buf[50];
    int rcv_count, flags;
    int REG_FLAG = 0;
    char buf[SIZE];

    scanner_count= 0;
    proj_count = 0;

   //find out the hostname and IP address
   if(gethostname(hostname, sizeof(hostname))==0)
   {
		if((entry=gethostbyname(hostname))!=NULL)
		{
			strcpy(SERVICE_ADDR_STR,inet_ntoa(*(struct in_addr *)entry->h_addr_list[0]));
			printf("IP Address =%s\n",SERVICE_ADDR_STR);
		}
   }

   // create a socket
   first_socket = socket(AF_INET,SOCK_STREAM,0);
   if (first_socket<0) {
      perror("error opening the first socket:");
      exit(1);
   }

   // bind it to a port and advertise it
   first_addr.sin_family = AF_INET;
   first_addr.sin_port = MY_PORT;
   first_addr.sin_addr.s_addr = INADDR_ANY;
   bind_status = bind(first_socket,(struct sockaddr *) & first_addr,sizeof(struct sockaddr_in));
   if (bind_status<0) {
      perror("error on binding first_socket:");
      exit(1);
   }

   // inform the system how many unserved connections are allowed
   listen_status = listen(first_socket,MY_UNSERVED);
   if (listen_status<0) {
      perror("error on listen for fist socket:");
      exit(1);
   }
   client_addr_len = sizeof(client_addr);

   // wait and accept connections
   printf("Waiting for connections\n");
   flags = 0;

   while (1)
   {
      memset(&client_addr, 0, sizeof(client_addr));
      second_socket = accept(first_socket, (struct sockaddr *)&client_addr, &client_addr_len);
      if (second_socket<0) {
         perror("error on accept on first socket:");
         exit(1);
      }

      //Connect from one service
      printf( "I accepted a connection from %s\n", inet_ntoa(client_addr.sin_addr) );

      //Receive the service request from devices
      rcv_count=recv(second_socket,buf,SIZE,flags);
      if(strcmp(buf,"registering")==0)
      {
    	  printf("registering\n");
    	  if(REG_FLAG==1)
    	  {
    		 strcpy(serv_id, "0");
    		 send_status = send(second_socket,serv_id,strlen(serv_id)+2,flags);
           	 continue;
          }

    	  REG_FLAG=1;

    	  //Registration not completed
    	  if(registration())
    	  {
    		  strcpy(serv_id, "0");
    	      send_status = send(second_socket,serv_id,strlen(serv_id)+2,flags);
    	      REG_FLAG=0;
    	      continue;
    	  }
    	  REG_FLAG=0;
      }

      //Receive request from client to discover a particular service
      else if(strcmp(buf, "discovering")==0)
      {
    	  printf("discovering\n");
    	  dis_buf[0]='\0';

    	  //Unable to discover device
    	  if(discovery(dis_buf)!=0)
			  strcpy(dis_buf, "Failed");

    	  //send a message to client that service is found
    	  send_status = send(second_socket,dis_buf,strlen(dis_buf)+2,flags);
    	  continue;
      }
   }
}

//Receive information from service that wants to register with the directory
int registration()
{
	unsigned char buf[SIZE], ack_buf[SIZE];
	int rcv_count, flags;

	flags=0;
	serv_id[0]='\0';
	buf[0]='\0';

	//Get the unique service id
	get_serviceid(serv_id);

	//send unique sevice_id
	send_status = send(second_socket,serv_id,strlen(serv_id)+2,flags);
	if (send_status<0)
	{
		perror("Could not send registration ack to the device\n");
		return 1;
	}

	//receive service information
	rcv_count=recv(second_socket,buf,SIZE,flags);

	//check if device is registered from before
	if(reg_done(buf))
	{
		strcpy(ack_buf, "Device is already registered!");
		goto REGISTERED;
	}

	//Add new service to the directory
	register_service(buf);
	strcpy(ack_buf, "Registered Successfully");

	//update service_id after successful registration
	update_servid();

REGISTERED:
	//send acknowledgement successfully registered
	send_status = send(second_socket,ack_buf,strlen(ack_buf)+2,flags);
	if (send_status<0)
	{
		perror("Registered but no ack to the device\n");
		return 1;
	}

	return 0;
}

//discover the requested service from the directory for the client
int discovery(char *dis_buf)
{
	unsigned char buf[SIZE];
	int rcv_count, flags,count ;
	flags=0;

	//receive service request information
	rcv_count=recv(second_socket,buf,SIZE,flags);

	//check the services requested by the client
	if(strcmp(buf,"scanner"))
		count = scanner_count++;
	else
		count = proj_count++;

	//find the requested service in directory
	if(find_service(buf,dis_buf,count)!=0)
		return 1;
	return 0;
}
