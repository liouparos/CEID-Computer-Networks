#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include "kvs.c"
#include "kvs.h"

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen, n, k, i,j, len;
	KVSstore *store = kvs_create(strcmp);
	KVSpair *p;
   char *buffer, *key, *value, *retvalue, *restoclient;
 	char *buffer2;
	buffer = malloc(1024);
	buffer2 = malloc(1024);
	key = malloc(50);
	value	= malloc(50);
	retvalue	= malloc(50);
	restoclient = malloc(50);
	bzero(buffer,1024);
   struct sockaddr_in serv_addr, cli_addr;

   
   /*create socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /*socket structure*/
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* bind */
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /*listening*/
   listen(sockfd,5);
   clilen = sizeof(cli_addr);


 while(1) {
   /* Accept */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }

   bzero(buffer,1024);
	bzero(buffer2,1024);
	int len=0;
	do{
		k = read( newsockfd,buffer,1024); 
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
		j++;
		bzero(key,50);
		bzero(value,50);
		if (j==1 && memcmp(pbuffer,"p",1) != 0 && memcmp(pbuffer,"g",1) != 0) {
			close(newsockfd);
		}
		
		if (memcmp(pbuffer,"p",1) == 0){  		/* put command */
			memcpy(key, pbuffer+1, strlen(pbuffer)-1);
			pbuffer = strchr(pbuffer,'\0')+1;
			memcpy(value, pbuffer, strlen(pbuffer)+1);
			kvs_put(store, key, value);
			retvalue = kvs_get(store, key);
			pbuffer = strchr(pbuffer,'\0')+1;

		}
		if (memcmp(pbuffer,"g",1) == 0){			/* get command */
			memcpy(key, pbuffer+1, strlen(pbuffer)-1);
			retvalue = kvs_get(store, "city");
			if (retvalue != '\0'){
				n = write(newsockfd,"f",strlen("f"));
				memcpy(restoclient,retvalue,strlen(retvalue)+1);
				len = (strlen(retvalue) +2);
				n = write(newsockfd,restoclient,len);
				
			}
			else {
				len = strlen("n");
				memcpy(restoclient,"n",len);
				n = write(newsockfd, restoclient,len);
			}
			pbuffer = strchr(pbuffer,'\0')+1;
			bzero(restoclient,50);
			
		}
		
		while ((p = kvs_pair(store, i++))) {
		printf("%s: %s\n", (char *)p->key, (char *)p->value);
		}
	
	 }	
  }
	close(newsockfd);
	kvs_destroy(store);
	close(sockfd);
   return 0;
}
