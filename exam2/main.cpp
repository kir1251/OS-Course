#include "msgstream.h"

#include <stdio.h>

int main(int argc, char** argv) 
{

    const char chars[8] = {'M', 'E', 'S', 'G', 0x08, 0x00, 0x00, 0x00};

    if (argc != 3)
    {
        fprintf(stderr, "error: not enough arguments. usage: msgstreamtest fromfd tofd");
    }

    int fd1 = atoi(argv[1]);
    int fd2 = atoi(argv[2]);
    FILE *f1 = fdopen(fd1, "r");
    FILE *f2 = fdopen(fd2, "w");
    pingpong pp = pingpong(fd2);
    fprintf(f1, "PINGPINGPONG");
    fprintf(f1, chars);
    fprintf(f1, "HelloLOLPINGPONGPONGEND");
    while (true) 
    {
        message* msg = pp.get();
        if (msg->type == msg->ERR) 
        {
            break;
        };
        if (msg->type == msg->MESG) 
        {
            printf(0, msg->messag);
        }
        free(msg->messag);
       free(msg); 
    }
    delete pingpong;
    return 0;
}

