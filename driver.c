/* Example: client.c sending and receiving datagrams using UDP */
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int sock;
struct sockaddr_in name;

/* client program called with host name and port number of server */
void main(int argc, char *argv[])
{
    int buflen;
    struct hostent *hp, *gethostbyname();

    if(argc < 3) {
	printf("usage: driver remote_host_name remote_port_number\n");
	exit(1);
    }

    /* create socket for connecting to server */
    sock = socket(AF_INET, SOCK_DGRAM,0);
    if(sock < 0) {
	perror("opening datagram socket");
	exit(2);
    }

    /* construct name for connecting to server */
    name.sin_family = AF_INET;
    name.sin_port = htons(atoi(argv[2]));

    /* convert hostname to IP address and enter into name */
    hp = gethostbyname(argv[1]);
    if(hp == 0) {
	fprintf(stderr, "%s:unknown host\n", argv[1]);
	exit(3);
    }
    bcopy((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);

    close(sock);
    exit(0);
    return;
}
