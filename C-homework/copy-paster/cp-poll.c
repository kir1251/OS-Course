#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>

const unsigned int BUFFER_MAX_SIZE = 128;

int main(int argc, char**argv) 
{
   if (argc % 2 != 0 || argc < 2) {
      printf("error: not enough arguments. Usage : copy-paster W1 R1 W2 R2 ...");
   }
   int n = argc / 2;
    
   struct pollfd* fds = malloc(argc * sizeof(struct pollfd));
   int i;
   for (i = 0; i < argc; i++) {
      fds[i].fd = atoi(argv[i]);
      fds[i].events = (i % 2 == 1) ? POLLIN : POLLOUT;
   }

   int * bu_size = malloc(n * sizeof(int));
   char ** buffer = malloc(n * sizeof(char));
   for (i = 0; i < argc; i++) {
       buffer[i] = malloc(BUFFER_MAX_SIZE * sizeof(char));
   }
   int working = n;
   while (working > 0) {
      int k = poll(fds, argc, 1000);
      if (k > 0) {
         for (i = 0; i < n; i ++) {
            if (fds[i * 2].revents & POLLIN && bu_size[i] < BUFFER_MAX_SIZE) {
               int d = BUFFER_MAX_SIZE - bu_size[i];
               if (d != 0) {
                  int nr = read(fds[i * 2].fd, buffer[i] + bu_size[i], d);
                  if (nr == 0) {
                      working--;
                  } else {
                     bu_size[i] += nr;
                  }
               }
            }
            if (fds[i * 2 + 1].revents & POLLOUT && bu_size[i] > 0) {
               int nw = write(fds[i * 2 + 1].fd, buffer[i], bu_size[i]);
               if (nw > 0) {
                   int rest = BUFFER_MAX_SIZE - nw;
                   memmove(buffer[i], buffer[i] + nw, rest);
                   bu_size[i] -= nw;
               }
            }
         } 
      }
   }
   free(fds);
   for (i = 0; i < argc; i++) {
       free(buffer[i]);
   }
   free(buffer);
   free(bu_size);
}
