#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
#include<iostream>
using namespace std;
#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 10000   //The port on which to send data
int main(void)
{
    struct sockaddr_in si_other;
    socklen_t slen=sizeof(si_other);
    int sockets, i=sizeof(si_other);
    char buf[BUFLEN];
    if ( (sockets=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)//creation socket client
    {
        perror("error socket");
        exit(1);
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    const char *sendTexte = "Phrase de test";// cas standard envoyer une seule data
        //envoyer le  message
        if (sendto(sockets, sendTexte, strlen(sendTexte) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            perror("error send");
            exit(1);
        }


        //effacer le contenu du buffer
        memset(buf,'\0', BUFLEN);
         //recevoir la donnée
        if (recvfrom(sockets, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            perror("error send");
            exit(1);
        }

        cout<<"la data qui à été envoyé et réenvoyée par le serveur : "<<buf<<endl;



    close(sockets);//pas de shutdown
    return 0;
}
