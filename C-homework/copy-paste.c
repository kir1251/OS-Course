#include <stdio.h>
#include <poll.h>
#include <stdlib.h>

int main(int argc, char**argv) 
{
   if (argc % 2 != 0 || argc < 2) {
      printf("error: not enough arguments. Usage : copy-paster W1 R1 W2 R2 ...");
   }
   
   struct pollfd* fds = malloc(argc * sizeof(struct pollfd));
   int i;
   for (i = 0; i < argc; i++) {
      fds[i].fd = atoi(argv[i]);
      fds[i].events = i%2 == 1 ? POLLIN : POLLOUT;
   }
   

   free(fds);
}
