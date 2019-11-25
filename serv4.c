#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include "kvs.c"
#include "kvs.h"

void *connection_handler(void *);

int main(int argc , char *argv[])
{
    int portno, socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
	 portno = atoi(argv[1]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portno);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

        c=sizeof(struct sockaddr_in);
       while(client_sock=accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c))
       {

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}

void *connection_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int n,k;
	 char sendBuff[100], client_message[2000];
	char *buffer, *key, *value, *retvalue, *restoclient;
 	char *buffer2;
	buffer = malloc(1024);
	buffer2 = malloc(1024);
	key = malloc(50);
	value	= malloc(50);
	retvalue	= malloc(50);
	restoclient = malloc(50);
	bzero(buffer,1024);
	KVSstore *store = kvs_create(strcmp);
	KVSpair *p; 
		
			/*.........................................................*/
			int len=0;
	do{
		k = read( sock,buffer,1024); 
  		if (k < 0) {
			perror("ERROR reading from socket");
			exit(1);
   	}
		memcpy(buffer2+len, buffer, k);
		len += k;
	}while (buffer == NULL);

		/*key-value store*/
		char *pbuffer = buffer2;
	while (*pbuffer){
			bzero(key,50);
			bzero(value,50);
		
		if (memcmp(pbuffer,"p",1) == 0){
			memcpy(key, pbuffer+1, strlen(pbuffer)-1);
			pbuffer = strchr(pbuffer,'\0')+1;
			memcpy(value, pbuffer, strlen(pbuffer)+1);
			kvs_put(store, key, value);
			pbuffer = strchr(pbuffer,'\0')+1;
			
			
		}
		if (memcmp(pbuffer,"g",1) == 0){
			memcpy(key, pbuffer+1, strlen(pbuffer)-1);
			retvalue = kvs_get(store, "city");
			if (retvalue != '\0'){
				n = write(sock,"f",strlen("f"));
				memcpy(restoclient,retvalue,strlen(retvalue)+1);
				len = (strlen(retvalue) +2);
				n = write(sock,restoclient,len);
				
			}
			else {
				len = strlen("n");
				memcpy(restoclient,"n",len);
				n = write(sock, restoclient,len);
			}
			pbuffer = strchr(pbuffer,'\0')+1;
			bzero(restoclient,50);
			
		} /*if*/
			
	 }	/*while (*pbuffer)*/
			/*.........................................................*/


      
    return 0;
}
