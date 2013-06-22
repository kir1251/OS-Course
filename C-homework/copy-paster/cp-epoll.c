#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const unsigned int BUFFER_MAX_SIZE = 128;
const unsigned int EVENTS_BUFFER_SIZE = 16;

typedef struct epoll_event epoll_event;

int main(int argc, char** argv) {
   if (argc % 2 != 0 || argc < 2) {
      printf("error: not enough arguments. Usage : copy-paster W1 R1 W2 R2 ...");
   }

   int n = argc / 2;
   int i;

   int epfd = epoll_create1(0);
   int* fds = malloc(argc * sizeof(int));
   for (i = 0; i < argc; i++) {
      int fd = atoi(argv[i]);
      fds[i] = fd;
      int ep_flag = (i % 2 == 0) ? EPOLLIN : EPOLLOUT;
      epoll_event e;
      e.events = ep_flag;
      e.data.fd = i;
      epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &e);      
   }

   int * bu_size = malloc(n * sizeof(int));
   char ** buffer = malloc(n * sizeof(char));
   for (i = 0; i < argc; i++) {
      buffer[i] = malloc(BUFFER_MAX_SIZE * sizeof(char));
   }

   int working = n;
   while (working > 0) {
      epoll_event *event_buffer = malloc(EVENTS_BUFFER_SIZE * sizeof(epoll_event));
      int events = epoll_wait(epfd, event_buffer, EVENTS_BUFFER_SIZE, 1000);
      for (i = 0; i < events; i++) {
         int id = event_buffer[i].data.fd;
         int rid = id/2;
         if (id % 2 == 0 && bu_size[rid] < BUFFER_MAX_SIZE && event_buffer[i].events & EPOLLIN) {
            int d = BUFFER_MAX_SIZE - bu_size[rid];
            if (d != 0) {
               int nr = read(fds[id], buffer[rid] + bu_size[rid], d);
               if (nr == 0) {
                  working--;
               } else {
                  bu_size[rid] += nr;
               }
            }
         }

         if (id % 2 == 0 && bu_size[rid] > 0 && event_buffer[i].events & EPOLLOUT) {
             int nw = write(fds[id], buffer[rid], bu_size[rid]);
             if (nw > 0) {
                int rest = BUFFER_MAX_SIZE - nw;
                memmove(buffer[rid], buffer[rid] + nw, rest);
                bu_size[i] -= nw;
             }
         }
                                                                                                        
      }
      free(event_buffer);
   }
   
   for (i = 0; i < argc; i++) {
      free(buffer[i]);
   }
   free(buffer);
   free(bu_size);
   free(fds);
}
