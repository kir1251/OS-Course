#include <stdlib.h>
struct message
{
    static const int ERR = 0;
    static const int PING = 1;
    static const int PONG = 2;
    static const int MESG = 3;
    
    int type;
    int length;
    char* messag;

    ~message()
    {
        free(messag);
    }
};

struct pingpong
{
    static const int BUFFER_MAX_SIZE = 128;
    char *buffer;
    int length;
    pingpong(int fd);
    ~pingpong();
    message* get();
    int fd;
};
