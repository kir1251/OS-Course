typedef struct message
{
    static const int ERRO = 0;
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
} message;

typedef struct pingpong
{
    static const int BUFFER_MAX_SIZE = 128;
    char *buffer;
    int length;
    pingpong(fd);
    ~pingpong();
    message* get();
    int fd;
} pingpong;
