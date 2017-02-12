#include <stdio.h>
    #include <sys/types.h>
       #include <sys/socket.h>
       #include <netdb.h>
       
       #include <stdlib.h>
       #include <unistd.h>
       #include <string.h>

#define CSTE 10

int main() {
    char nomlocal[CSTE + 1];
    int res;
    res = gethostname(nomlocal, CSTE);
    printf("nomlocal = %s", nomlocal);
   
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    
   
    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0; /* Any protocol */

    s = getaddrinfo(nomlocal, NULL, &hints, &result);


    return 0;
}
