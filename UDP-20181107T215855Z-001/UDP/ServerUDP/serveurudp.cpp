
#include <QCoreApplication>
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 10000   //The port on which to listen for incoming data


int main(void)
{
    struct sockaddr_in si_me, si_other;

    int sockets, i = sizeof(si_other) , recv_len;
    socklen_t slen = sizeof(si_other);
    char buf[BUFLEN];

    //creation du socket serveur
    if ((sockets=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {

        perror("error socket");
        exit(1);
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(sockets , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        perror("error bind");
        exit(1);
    }

    //keep listening for data boucle pour rester à l'ecoute
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(sockets, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            perror("error recv");
            exit(1);
        }

        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

        //réenvoyer le meme message
        if (sendto(sockets, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            perror("error send to ");
            exit(1);
        }
        memset(buf,0 ,BUFLEN);
    }

    close(sockets);// pas de shutdown
    return 0;
}
