#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include "kvs.c"
#include "kvs.h"


int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen, num, k, i,j;
   
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
   int n, pid;
	KVSstore *store = kvs_create(strcmp);
	KVSpair *p;  
	
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname proccesses\n", argv[0]);
      exit(0);
   }
   num = atoi(argv[2]);
	printf("num = %d\n",num);
	
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (num==0) close(sockfd);
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   
   /* Now start listening for the clients, here  */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   while (1) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }
      for (int i=0; i<num; i++) {
	      /* Create child process */
	      pid = fork();
			
	      if (pid < 0) {
	         perror("ERROR on fork");
	         exit(1);
	      }
	      
	      if (pid == 0) {
	         /* This is the client process */
	         close(sockfd);
				/*.........................................................*/
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
			bzero(key,50);
			bzero(value,50);
			if (j==1 && memcmp(pbuffer,"p",1) != 0 && memcmp(pbuffer,"g",1) != 0) {
				close(newsockfd);
				close(sockfd);
				return 0;
			}
		
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
			
		} /*if*/
			
	 }	/*while (*pbuffer)*/
			/*.........................................................*/
      	   exit(0);
      	}
      	else {
      	   close(newsockfd);
      	}
		} /* for */
   } /* while */
}
