/*
 * main.c
 *Service program
 *  Created on: Oct, 2015
 *      Author: manisha agrawal
 */

#include "service.h"

int main(int argc, char * argv[]) {
   struct sockaddr_in first_addr;
   char dir_IP[SIZE], buf[SIZE], serv_id[SIZE];
   char timestamp[100], serv_inst[SIZE], message[SIZE];
   int send_status,connect_status,rcv_count,flags;
   int first_socket;
   char SERVICE_ADDR_STR[100];
   int REG_FLAG=0;

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

   // from time to time send some data
   flags = 0;
   rcv_count=0;

   //Get the timesptamp
   format_time(timestamp);

   while (1)
   {
	   //*************Registration stage*****************//
	  if(REG_FLAG==0)
	  {
		  strcpy(message,"registering");
		  send_status = send(first_socket,message,strlen(message)+2,flags);
		  if (send_status<0)
		  {
				   perror("Could not register the device\n");
				   system("tclsh message.tcl \"Could not register the device\"");
				   exit(1);
		  }
		  usleep(SLEEP_TIME); // wait a while

		  //Wait for service_id from server
		  rcv_count=recv(first_socket,serv_id,SIZE,flags);
		  if(rcv_count<=0)
		  {
			  perror("error on connecting with the Server!!");
			  system("tclsh message.tcl \"Could not register the device\"");
			  //exit(1);
			  continue;
		  }

		  if(strcmp(serv_id,"0") == 0)
		  {
			   perror("Server is busy registering other device!!");
			   system("tclsh message.tcl \"Server is busy registering other device!!\"");
			   //exit(1);
		  }

		  //send service details to the directory
		  sprintf(serv_inst, "%s is active at %s\n",SERVICE_NAME, SERVICE_ADDR_STR );
		  sprintf(buf,"serviceid:%s\nservicename:%s\nservicetypeid:%s\nIPaddress:%s\nport:%d\ntimestamp:%s\nservicestatus:%d\nlifespan=%d\ninstructions:%s\n",\
				  serv_id,SERVICE_NAME,SERVICE_TYPEID,SERVICE_ADDR_STR,MY_PORT1,timestamp,SERVICE_STATUS,LIFESPAN_HARD,serv_inst);
printf("<%s>",buf);
		  usleep(SLEEP_TIME); // wait a while

		  // send buffer though socket
		  send_status = send(first_socket,buf,strlen(buf)+2,flags);
		  if (send_status<0) {
			 system("tclsh message.tcl \"Could not register the device\"");
			 //exit(1);
		  }

		  //Registered successfully
		  rcv_count=recv(first_socket,buf,SIZE,flags);
		  if(rcv_count<=0)
		  {
			perror("No ack received from the Server!!");
  		    system("tclsh message.tcl \"Could not register the device\"");
   		    //exit(1);
		 }
		  if(strcmp(buf,"Registered Successfully")==0)
		  {
			  system("tclsh message.tcl \"Device Registered Successfully\"");
			  REG_FLAG=1;
			  //continue;
		  }
		  else if(strcmp(buf,"Device is already registered!")==0)
		  {
			  system("tclsh message.tcl \"Device is already registered!\"");
			  REG_FLAG=1;
			  //continue;
		  }
		  else
		  {
			   system("tclsh message.tcl \"Could not register the device\"");
			   //continue;
		  }
	   }

	  //start the service for a client
	  start_service();
   }
   return 0;
}
