/*
 * service.h
 *
 *  Created on: Oct 11, 2015
 *      Author: manisha agrawal
 */

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#define MY_PORT        4567 // port number
#define MY_PORT1       1234 // port number

#define SIZE           1024
#define MY_UNSERVED    5
#define SLEEP_TIME     1000000
#define SERVICE_STATUS 1   //Set '0' for soft state, '1' for hard state
#define LIFESPAN_SOFT  300 //Set lifespan in seconds
#define LIFESPAN_HARD  0   //Lifespan not needed for hard state

//#define RLC 		   "192.168.1.122" //directory IP address

#define SERVICE_NAME   "scanner"
#define SERVICE_TYPEID "sc"
#define DIR_IP_FILE  	"dirIP.txt"

/*****************FUNCTIONS*****************/
void format_time(char *);
int start_service();
int get_serverip();
