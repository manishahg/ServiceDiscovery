/*
 ============================================================================
 Name        : discovery.c
 Author      : manisha agrawal
 Copyright   : Your copyright notice
 ============================================================================
 */

#include "discovery.h"

int main(int argc, char * argv[]) {
   struct sockaddr_in first_addr;
   char buf[SIZE],  sys_msg[SIZE],dir_IP[100], portno[SIZE];
   int send_status,connect_status,rcv_count,flags;
   int iteration, first_socket;
   char SERVICE_ADDR_STR[100];
   int DIS_FLAG=0;
   char service_ip[SIZE];

   //find out the hostname and IP address
   char hostname[50];
   struct hostent *entry;
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

   dir_IP[0]='\0';
   if(get_serverip(dir_IP))
   {
	   perror("error getting Directory IP");
   	   exit(1);
   }

   // connect to it;
   first_addr.sin_family = AF_INET;
   first_addr.sin_port = MY_PORT;
   first_addr.sin_addr.s_addr = inet_addr(dir_IP);
   connect_status = connect(first_socket,(struct sockaddr *) & first_addr,sizeof(struct sockaddr_in));
   if (connect_status<0) {
      perror("error on connecting first_socket:");
      exit(1);
   }

   flags = 0;
   iteration = 0;
   rcv_count=0;
   char message[SIZE];

   while (1)
   {
	   //*************Discovery stage*****************//
	  if(DIS_FLAG==0)
	  {
		  strcpy(message,"discovering");
		  send_status = send(first_socket,message,strlen(message)+2,flags);
		  if (send_status<0)
		  {
				   perror("Could not connect to the Server!!\n");
				   system("tclsh message.tcl \"Could not connect to the Server!!\"");
				   exit(1);
		  }
		  usleep(SLEEP_TIME); // wait a while

		  //Sending request for particular service
		  message[0]='\0';
		  get_service(message);
		  send_status = send(first_socket,message,strlen(message)+2,flags);
		  usleep(SLEEP_TIME); // wait a while

		  if (send_status<0) {
			 perror("Could not connect to the Server!!");
			 system("tclsh message.tcl \"Could not connect to the Server!!\"");
			 exit(1);
		  }

		  //Discovered successfully
		  buf[0]='\0';
		  rcv_count=recv(first_socket,buf,SIZE,flags);

		  if(rcv_count<=0)
		  {
			perror("No ack received with the Server!!");
			system("tclsh message.tcl \"Service not available!!\"");
			exit(1);
		 }

		  if(strcmp(buf,"Failed")==0)
		  {
			  system("tclsh message.tcl \"Service not available!!\"");
			  exit(1);
		  }

		  //Save service information and get service IP addr
		  sprintf(sys_msg, "tclsh message3.tcl \"%s\"",buf);
		  system(sys_msg);
		  save_service(buf,service_ip, portno);
		  int server_port = atoi(portno);

		  //**************Invocation  Stage********************/
		  //Check if client wants to invoke the service
		  if(invoc())
			  exit(1);

		  //invoke the service
		  if(invoke(service_ip, SERVICE_ADDR_STR, server_port))
		  {
			  sprintf(sys_msg, "tclsh message.tcl \"Unable to connect %s\"",message);
			  system(sys_msg);
			  exit(1);
		  }
	   }
   }
}
