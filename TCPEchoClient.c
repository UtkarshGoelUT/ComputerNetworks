#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);
void DieWithError(char *errorMessage)
{
    perror (errorMessage) ;
    exit(1);
}
/* Size of receive buffer */

/* Error handling function */

int main(int argc, char *argv[])
{
    int sock; /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;
    char *servlP;
    char *echoString;
    char echoBuffer[RCVBUFSIZE];
    unsigned int echoStringLen;
    int bytesRcvd, totalBytesRcvd;

    /* Echo server port */
    /* Server IP address (dotted quad) */
    /* String to send to echo server */
    /* Buffer for echo string */
    /* Length of string to echo */
    /* Bytes read in single recv()
    and total bytes read */

    if ((argc< 3) || (argc> 4)) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
        argv[0]);
        exit(1);
    }

    servlP = argv[1] ;
    echoString = argv[2] ;

    /* First arg' server IP address (dotted quad) */
    /* Second arg' string to echo */

    if (argc == 4)
        echoServPort = atoi(argv[3]); /* Use given port, if any */
    else
        echoServPort = 7; /* 7 is the well-known port for the echo service */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError(" socket () failed") ;


    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servlP); /* Server IP address */
    echoServAddr.sin_port = htons(echoServPort); /* Server port */


    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError(" connect () failed") ;
    echoStringLen = strlen(echoString) ; /* Determine input length */

    /* Send the string to the server */
    if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
        DieWithError("send() sent a different number of bytes than expected");

    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    printf("Received: "); /* Setup to print the echoed string */


    while (totalBytesRcvd < echoStringLen)
    {
        /* Receive up to the buffer size (minus i to leave space for
        a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
        echoBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
        printf(echoBuffer); /* Print the echo buffer */
    }

    printf("\n"); /* Print a final linefeed */
    close(sock);
    exit(0);
}