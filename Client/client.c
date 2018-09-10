#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>


int* bin(char sym)
{
  float x = (float)sym;
  float ost = x;
  int *bin;
  bin = (int*)malloc(8 * sizeof(int));
  int tick = 0;
  while (tick < 8)
  {
    bin[tick] = 0;
    tick++;
  }

  if (x <= 127)
  {
    int i = 7;
    while (ost > 2)
    {
      int temp = (int)ost % 2;
      ost /= 2;
      bin[i] = temp;
      i--;
    }
    bin[i] = (int)ost;
  }
  return bin;
}

void len_manip(char *name, int *mes, FILE *f1)
{
  int s, result, N = 20, nums1, counter = 0, bit;
  char pattern[N+1], buff[] = "END386";;
  nums1 = N;
  s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (s < 0)
  {
      perror("Error calling socket\n");
      return;
  }
  struct sockaddr_in peer;
  peer.sin_family = AF_INET;
  peer.sin_port = htons(18666);
  peer.sin_addr.s_addr = inet_addr("127.0.0.1");
  result = connect(s, (struct sockaddr *)&peer, sizeof(peer));
  if (result)
  {
      perror("Error calling connect\n");
      return;
  }
  result = send(s, name, strlen(name), 0);
  if (result <= 0)
  {
      perror("Error  calling send");
      return;
  }

  while (nums1 == N)
  {
    memset(pattern, 0, sizeof(char)*N+1);
    bit = mes[counter % (8*100)]; /*100 - length oh secret message*/
    nums1 = fread(pattern, sizeof(char), N + bit, f1);
    if (nums1 == 0)
      break;
    result = send(s, pattern, N + 1, 0);
    if (result <= 0)
    {
        perror("Error  calling send");
        return;
    }
    nums1 -= bit;
    counter++;
  }

  result = send(s, buff, 13, 0);
  if (result <= 0)
  {
      perror("Error  calling send");
      return;
  }
  if (shutdown(s, 1) < 0)
  {
      perror("Error calling shutdown");
      return;
  }
}

void time_manip(char *name, int *mes, FILE *f1)
{
  int s, result, N = 20, nums1, counter = 0, bit;
  char pattern[N+1], buff[] = "END386";
  nums1 = N;
  s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (s < 0)
  {
      perror("Error calling socket\n");
      return;
  }
  struct sockaddr_in peer;
  peer.sin_family = AF_INET;
  peer.sin_port = htons(18666);
  peer.sin_addr.s_addr = inet_addr("127.0.0.1");
  result = connect(s, (struct sockaddr *)&peer, sizeof(peer));
  if (result)
  {
      perror("Error calling connect\n");
      return;
  }
  result = send(s, name, strlen(name), 0);
  if (result <= 0)
  {
      perror("Error  calling send");
      return;
  }

  while (nums1 == N)
  {
    memset(pattern, 0, sizeof(char)*N+1);
    bit = mes[counter % (8*100)]; /*100 - length of the secret message*/
    sleep(bit);
    nums1 = fread(pattern, sizeof(char), N, f1);
    if (nums1 == 0)
      break;
    result = send(s, pattern, N + 1, 0);
    if (result <= 0)
    {
        perror("Error  calling send");
        return;
    }
    counter++;
  }

  result = send(s, buff, 13, 0);
  if (result <= 0)
  {
      perror("Error  calling send");
      return;
  }
  if (shutdown(s, 1) < 0)
  {
      perror("Error calling shutdown");
      return;
  }
}

char *optarg;

int main(int argc, char* argv[])
{
  int opt, optind, opterr, optopt, stop, s_len = 100, *temp, *mes, choice, tick, tack;
  char a[s_len], *name;
  FILE *f1, *f2;

  if (argc != 2 && argc != 5)
  {
    printf("Incorrect number of arguments\n");
    printf("Use -h for more information\n");
    return(0);
  }

  while ((opt = getopt(argc, argv, "f:s:h")) != -1)
  {
    switch(opt)
    {
      case 'f':
        f1 = fopen(optarg, "rb");
        name = optarg;
        break;
      case 's':
        choice = atoi(optarg);
        break;
      case 'h':
        printf("Usage example: ./client -f example.txt -s 1\n");
        printf("Where -f is path to your file\n");
        printf("And -s is mode of HLC\n");
        printf("You can use -s 1 or -s 2\n");
        printf("1 - manipulating with length of packets\n");
        printf("2 - manipulating with time between packets\n");
        printf("Max length of the secret message is 100 chars\n");
        printf("The secret message store in client_secret.txt\n");
        return(0);
        break;
    }
  }
  if (!f1){
    printf("File not found!\n");
    return (0);
  }
  if (choice != 1 && choice != 2)
  {
    printf("Incorrect -s parametr. Use -h for more information.\n");
    return (0);
  }

  f1 = fopen(name, "rb");
  f2 = fopen("client_secret.txt", "rb");
  fread(a, sizeof(char), s_len, f2);
  mes = (int*)malloc(8 * s_len * sizeof(int));


  tick = 0;
  while (tick < s_len)
  {
    temp = bin(a[tick]);
    tack = 0;
    while (tack < 8)
    {
      mes[tick*8 + tack] = temp[tack];
      tack++;
    }
    tick++;
  }

  if (choice == 1)
    len_manip(name, mes, f1);
  else if (choice == 2)
    time_manip(name, mes, f1);

  return 0;

}
