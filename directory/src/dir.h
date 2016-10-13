/*
 * dir.h
 *Projecy : Directory
 * Created on: Oct  2015
 * Author: manisha agrawal
 */

#include <sys/socket.h> // socket header files
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define MY_PORT      4567
#define SIZE         1024
#define MY_UNSERVED  5
#define SERVICE_FILE "services.txt"
#define SERVICE_ID   "service_id.txt"


//***********************Functions**********************************//
void get_serviceid(char *);
void update_servid();
void register_service(char *);
int reg_done(char *);
int registration();
int discovery(char *);
int find_service(char *, char *,int);

