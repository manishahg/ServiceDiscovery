/*
 * discovery.h
 *
 *  Created on: Oct 11, 2015
 *      Author: manisha
 */


#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#define MY_PORT      4567 // port number for directory
//#define MY_PORT1     1234 // port number for service device

#define SIZE         1024
#define MY_UNSERVED  5
#define SLEEP_TIME   200000
#define SERVICE "service_file.txt" //service wanted from Server
#define DEFAULT_SERVICE "scanner"
#define SERVICE_INFO "service_info.txt"
#define INVOC_FILE "invoc.txt"
#define DIR_IP_FILE  	"dirIP.txt"

int get_serverip(char *);
int get_service(char *);
int save_service(char *, char *, char *);
int invoc();
int invoke(char *, char *, int);
