/*
 * functions.c
 *
 *  Created on: Oct 11, 2015
 *      Author: manisha
 */
#include "dir.h"

//Get a unique service id to assign to a service
void get_serviceid(char *serv_id)
{
	FILE *fp;
	fp = fopen(SERVICE_ID,"r"); // read mode

	//If file doesnot exist create it and set serv_id=1
	if( fp == NULL )
	{
		fp=fopen(SERVICE_ID,"w");
		fputs("1",fp);
		strcpy(serv_id, "1");
		fclose(fp);
		return;
	}

	fgets ( serv_id, sizeof(serv_id), fp ) ;
	fclose(fp);
	return ;
}

//Check if a service is registered from before
int reg_done(char *buf)
{
	FILE *fp;
	char msg[1024], serv_id[1024];
	int i=0;

	fp = fopen(SERVICE_FILE,"r"); // read mode
	if( fp == NULL )
		return 0;

	char *p = strstr(buf,"instructions:");
	while(*p != '\n')
		serv_id[i++] = *p++;
	serv_id[i++]='\n';
	serv_id[i]='\0';

	//check if any other device has same service name and ip address
	while(fgets(msg, SIZE, fp) != NULL)
	{
		if(strstr(msg,"instructions:")!=0)
		{
			if(strcmp(serv_id,msg))
				continue;
			fclose(fp);
			return 1;
		}
	}

	fclose(fp);
	return 0;
}

//Update the service id each time a new service is registered
void update_servid()
{
		FILE *fp;
		char serv_id[100];
		int id;

		fp = fopen(SERVICE_ID,"r+"); // read mode

		//If file doesnot exist create it and set serv_id=1
		if( fp == NULL )
		{
				fp=fopen(SERVICE_ID,"w");
				fputs("1", fp);
				fclose(fp);
				return;
		}

		fgets ( serv_id, sizeof(serv_id), fp ) ;
		id=atoi(serv_id);
		id++;
		sprintf(serv_id, "%d", id);

		rewind(fp);
		fputs(serv_id, fp);
		fclose(fp);
		return ;
}

//Register a new service to the directory
void register_service(char *buf)
{
	FILE *fp;
	fp = fopen(SERVICE_FILE,"a"); // read mode

	//If file doesnot exist create it
	if( fp == NULL )
		fp=fopen(SERVICE_FILE,"w+");

	fputs(buf, fp);
	fclose(fp);
	return ;
}

//Find a service for a client
int find_service(char *buf, char *dis_buf,int count)
{
	FILE *fp;
	char msg[SIZE];
	char info[SIZE];

	fp = fopen(SERVICE_FILE,"r"); // read mode

	//Check if file  exist
	if( fp == NULL )
		return 1;

	while(fgets(msg, SIZE, fp) != NULL)
	{
		if(strstr(msg, buf) == NULL) //look for requested service
			continue;
		else
		{
			if(count--)
				continue;
			//Send service and ip addr
			info[0]='\0';
			strcpy(info,buf);
			fgets(msg, SIZE, fp);
			fgets(msg, SIZE, fp); //get ip address of service
			strcat(info, msg);
			fgets(msg, SIZE, fp); //get port number of service
			strcat(info, msg);
			strcpy(dis_buf,info);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);
	return 1;
}
