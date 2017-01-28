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

// Added the structure of the timer message
typedef struct timermessage{
    int action;
    int sequence_number;
    float time;
}timermessage;

// Cancel a timer function for a sequence number
void canceltimer(int sequence_number){
    timermessage msg;

    msg.action = 2;
    msg.sequence_number = sequence_number;
    msg.time = 0;
    if(sendto(sock, (char*)&msg, sizeof msg, 0, (struct sockaddr *)&name, sizeof(name)) <0) {
        perror("sending datagram message");
        exit(4);
    }
}

// Set a timer function for a specific sequence number and a specified amount of time
void settimer(float time, int sequence_number){
    timermessage msg;

    msg.action = 1;
    msg.sequence_number = sequence_number;
    msg.time = time;
    if(sendto(sock, (char*)&msg, sizeof msg, 0, (struct sockaddr *)&name, sizeof(name)) <0) {
        perror("sending datagram message");
        exit(4);
    }
}
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

    /* send message to server */
    int i;
    int sequence_numbers[] = {1,2,3,1,10,20,30};
    int sleeptimes[] = {1,1,1,1,1,1,1};
    float times[] = {18,5,14,0,3,5,1};
    int setorcancel[] = {1,1,1,2,1,1,1};
    for(i=0;i<(sizeof(times)/sizeof(float));i++){
        if(setorcancel[i] == 1){
            printf("Setting a timer for %f seconds with sequence no %d\n", times[i], sequence_numbers[i]);
            settimer(times[i], sequence_numbers[i]);
        }
        else{
            printf("Cancel a timer with sequence no %d\n", sequence_numbers[i]);
            canceltimer(sequence_numbers[i]);
        }
        sleep(sleeptimes[i]);
    }

    close(sock);
    exit(0);
    return;
}
