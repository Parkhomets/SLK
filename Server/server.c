#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <errno.h>
#include <time.h>


int main(){
    FILE *f3, *f4, *f5;
    long int t1, t2;
    int name = 0, sock, counter = 0, s1, rc, bit, time_temp = 0;
    struct sockaddr_in addr;
    char buf[22];

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0)
    {
        perror("Error calling socket!\n");
        return(0);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18666);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error calling bind\n");
        return(0);
    }
    if (listen(sock, 1))
    {
        perror("Error calling listen\n");
        return(0);
    }

    f4 = fopen("secret.txt", "w");
    f5 = fopen("secret2.txt", "w");
    t1 = time(0);
    for(;;)
    {
      s1 = accept(sock, NULL, NULL);
      if (s1 < 0)
      {
        perror("Error calling accept\n");
        return 0;
      }
      for (;;)
      {
          t2 = t1;
          t1 = time(0);
          memset(buf, 0, sizeof(char)*22);
          rc = recv(s1, buf, 21, 0);
          if (rc < 0)
          {
              if (errno == EINTR)
                  continue;
              perror("Can't receive data\n");
              return 0;
          }
          if (rc == 0)
              break;
          if(strncmp(buf, "END386", 6) == 0)
            {
              printf("Got data. Success.\n");
              return (0);
            }

          bit = strlen(buf) - 20;
          if (name == 0)
          {
            f3 = fopen(buf, "w");
          }
          
          if (name > 0)
          {
            if (bit >= 0)
              fprintf(f4, "%d", bit);
            if (time_temp == 1)
              fprintf(f5, "%d", t1 - t2);
            fprintf(f3, buf);
            time_temp = 1;
          }
          name = 1;
      }
    }

    return (0);

}
