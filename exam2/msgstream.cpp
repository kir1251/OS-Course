#include "msgstream.h"
#include "functions.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>


pingpong::pingpong(int fd)
{
    this->fd = fd;
    buffer = (char*)malloc(BUFFER_MAX_SIZE * sizeof(char));
    length = 0;
}

pingpong::~pingpong()
{
    free(buffer);

message* pingpong::get()
{
    int remain = BUFFER_MAX_SIZE - length;
    while (length == 0)
    {
        int r =read(fd, buffer + length, remain);
        if (r > 0) {
            length += r;
        }
    }
    for (int i = 0; i < FUNC_SIZE)
    {
        if (memcmp(buffer, FUNCTIONS[i].prefix, FUNCTIONS[i].prefix_length))
        {
            length -= FUNCTIONS[i].prefix_length;
            memmove(buffer, buffer + FUNCTIONS[i].prefix_length, length);
            return FUNCTIONS[i].func(fd, buffer, length);
        }
    }
}

