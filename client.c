#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <sys/ioctl.h>
		

int main(int argc, char *argv[]) {
   int sockfd, portno, n, k, i, j, l, rv, flag,len;
	int blen2 = 0;
	int blen = 0;
   struct sockaddr_in serv_addr;
   struct hostent *server;
	int size = argc*50+1;
   char *buffer, *buffer2, *buffer3;
	buffer = malloc(size); 
	buffer2 = malloc(500);
	buffer3 = malloc(50);


   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
	
   portno = atoi(argv[2]);
   
   /* Create socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	
   server = gethostbyname(argv[1]);
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   /* connect to server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
	/* Copy message from the user to buffer, this message will be read by server */
	for (i=3; i<argc; i++) {
		
 		if (strcmp(argv[i],"get") == 0) {
			flag=1;
			l+=1;
			if (argv[i+1] == NULL) goto ekei;
			memcpy(buffer+blen+blen2,"g",strlen("g"));
			memcpy(buffer+blen+strlen("g")+blen2, argv[i+1], strlen(argv[i+1])+1);
			blen = (strlen(argv[i+1])+strlen("g"));
			k=1;
			n = write(sockfd, buffer, blen+1);
			bzero(buffer,blen);	
			blen=0;		
   		if (n < 0) {
     	 		perror("ERROR writing to socket");
      		exit(1);
  			}
		}
		else if (strcmp(argv[i],"put") == 0) {
			if (argv[i+1] == NULL || argv[i+2] == NULL) goto ekei;
			memcpy(buffer+blen+blen2,"p",strlen("p"));
			memcpy(buffer+blen+strlen("p")+blen2, argv[i+1], strlen(argv[i+1]));
			memcpy(buffer+blen+strlen("p")+strlen(argv[i+1])+blen2+1, argv[i+2], strlen(argv[i+2])+1);
			blen = (strlen(argv[i+2])+strlen("p")+strlen(argv[i+1])+1);
			n = write(sockfd, buffer, blen+1);
			bzero(buffer,blen);
			blen=0;	
			if (n < 0) {
     	 		perror("ERROR writing to socket");
      		exit(1);
  			}
		}
		else if (strcmp(argv[3],"put") != 0 ) {
			if (strcmp(argv[3],"get") != 0)  goto ekei; 
		}
	}	/* for (i=3; i<argc; i++) */

	if (j==1) {
		ekei:
		j=1;
		close(sockfd);
	}

	bzero(buffer,size);
	if (flag==1){
		i=0;
		while (i<l){
			i++;
			bzero(buffer,size);
			k = read(sockfd, buffer, size); /* read servers response to get */
		 	if (k < 0) {
				perror("ERROR reading from socket");
   	  		exit(1);
   		}
			memcpy(buffer2+len, buffer, k);
			len += k;
			if (memcmp(buffer,"n",1) == 0) break;
		} /* while(i<l) */

		char *pbuffer = buffer2;
		while (*pbuffer){
			if (memcmp(pbuffer,"f",1) == 0){
   			l++;
				memcpy(buffer3,pbuffer+1,strlen(pbuffer));
				printf("%s\n",buffer3); 
				pbuffer = strchr(pbuffer,'\0')+1;
			}
			else if (memcmp(pbuffer,"n",1) == 0){
				l++;
				printf("\n");
				pbuffer = strchr(pbuffer,'n')+1;
			}
		} /* while(*pbuffer) */
	} /* if(flag==1) */	
	close(sockfd);
	free(buffer);
   return 0;
}


