all: serv1 serv2 serv3 serv4 client
serv1: serv1.c client.c
	gcc serv1.c -o serv1
	gcc client.c -o client

serv2: serv2.c
	gcc serv2.c -o serv2

serv3: serv3.c
	gcc serv3.c -o serv3

serv4: serv4.c
	gcc -pthread serv4.c -o serv4
