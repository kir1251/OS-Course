#include "msgstream.h"

struct msg_type
{
    char* prefix;
    int prefix_length;
    message* (func)(int, char*, int);
};

const msg_type FUNCTIONS[3];
const int FUNC_SIZE;


message* ping(int fd, char* buf, int &length) 
{
    message *m = (message*)malloc(sizeof(message));
    m->type = m->PING;
    m->length = 0;
    m->messag = NULL;
    return m;
}

message* pong(int fd, char* buf, int &length)
{
    message *m = (message*)malloc(sizeof(message));
    m->type = m->PING;
    m->length = 0;
    m->messag = NULL;
    return m;
}


message* mesg(int fd, char* buf, int &length)
{
    int size;
    memcpy(&size, buffer + 4, sizeof(int));
    char* ans = (char*)malloc(size + 1);
    length -= sizeof(int);
    if (size < length)
    {
        memcpy(ans, buffer + 4 + sizeof(int), size);
        length -= size;
        memmove(buffer, buffer + size + sizeof(int), length);
    } else 
    {
        memcpy(ans, buffer + 4 + sizeof(int), length);
        int sl = length;
        while (size > sl) {
        int i = read(fd, buffer, BUFFER_MAX_SIZE);
        if (size - sl > i) {
        memcpy(ans, buffer, i);
        sl += i; 
    } else
    {   
        memcpy(ans, buffer, size - sl);
        memmove(buffer, buffer + size - sl, BUFFER_MAX_SIZE - size + sl);
        length = BUFFER_MAX_SIZE - size + sl;
        sl = size;
        }
    }
    ans[size] = 0;

    message *m = (message*)malloc(sizeof(message));
    m->type = m->MESG;
    m->length = size;
    m->messag = ans;
    return m;
}
