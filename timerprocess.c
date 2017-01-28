#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TIMERPORT 1030
#define TCPDPORT 10809


bool lockres = false;
// Creating tcpd socket
struct sockaddr_in tcpd_socket;
int sock;

// Creating custom data types for network communication
typedef struct timedout{
    char action;
    int sequence_number;
}timedout;

typedef struct timermessage{
    int action;
    int sequence_number;
    float time;
}timermessage;

typedef struct timernode{
    int sequence_number;
    float time;
    struct timernode* next;
}timernode;

void TimerSet(int, int);
void TimerStop(int signum) {
    if(head == NULL) {printf("Why is it null ? \n"); return;}
    printf("\nTimer ran out! Stopping timer for sequence no %d\n", head->sequence_number);
    timedout msg;
    msg.action = '3';
    msg.sequence_number = head->sequence_number;
    sendto (sock, (char *) &msg, sizeof msg, 0,
        (struct sockaddr *) &tcpd_socket,
        (socklen_t) sizeof (struct sockaddr_in));
    return;
}
void TimerSet(int interval, int microseconds) {
    struct itimerval it_val;

    printf("interval : %d, microseconds : %d\n", interval,microseconds + 1000 > 999999 ? 999999 : microseconds + 1000);
    fflush(stdout);
    it_val.it_value.tv_sec = interval;
    it_val.it_value.tv_usec = microseconds + 1000 > 999999 ? 999999 : microseconds + 1000;
    it_val.it_interval.tv_sec = 0;
    it_val.it_interval.tv_usec = 0;

    if (signal(SIGALRM, TimerStop) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
    }
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }
}

int main(int argc, char *argv[]){
    int namelen;
    struct sockaddr_in name;
    //Tcpd related
    tcpd_socket.sin_family = AF_INET;
    tcpd_socket.sin_addr.s_addr = INADDR_ANY;
    tcpd_socket.sin_port = htons (TCPDPORT);

    /*create socket*/
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
	perror("opening datagram socket");
	exit(1);
    }

    /* create name with parameters and bind name to socket */
    name.sin_family = AF_INET;
    name.sin_port = htons(TIMERPORT);
    name.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
        perror("getting socket name");
        exit(2);
    }
    namelen=sizeof(struct sockaddr_in);
    /* Find assigned port value and print it for client to use */
    if(getsockname(sock, (struct sockaddr *)&name, &namelen) < 0){
        perror("getting sock name");
        exit(3);
    }
    printf("Server waiting on port # %d\n", ntohs(name.sin_port));

    /* waiting for connection from client on name and print what client sends */
    namelen = sizeof(name);
    timermessage rmsg;
    int test=0;
    while(1){
    test++;
    if(recvfrom(sock, &rmsg, sizeof rmsg, 0, (struct sockaddr *)&name, &namelen) < 0) {
        perror("error receiving"); 
        exit(4);
    }
    if(rmsg.action == 1){
        printf("\n%d : Setting the timer for %f seconds with sequence no %d\n", test, rmsg.time, rmsg.sequence_number);
        // Setting a timer. Yet to define
        settimer(&head, rmsg.sequence_number, rmsg.time);
    }
    else if(rmsg.action == 2){
        printf("\n%d : Cancelling timer sequence no %d\n", test, rmsg.sequence_number);
        // Cancelling a timer. Yet to define
        canceltimer(&head, rmsg.sequence_number);
    }
    }

    /* server terminates connection, closes socket, and exits */
    close(sock);
    exit(0);
    return 0;
}
