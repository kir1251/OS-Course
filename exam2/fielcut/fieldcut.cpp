#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h> //for debug only

const int BUFFER_DEFAULT_SIZE = 128;

struct column_text
{
    char* text;
    int length;

    void update(char* text, int len) 
    {
        int new_size = length + len;
        if (length == 0)
        {
            char* new_text = (char*)malloc(BUFFER_DEFAULT_SIZE * sizeof(char));
            memcpy(new_text, text, len);
            this->text = new_text;
            length = new_size;
            return;
        }
        int buk = new_size / BUFFER_DEFAULT_SIZE + 1;
        char* new_text = (char*)malloc(BUFFER_DEFAULT_SIZE * buk * sizeof(char));
        memcpy(new_text, this->text, length);
        memcpy(new_text + length, text, len);
        free(this->text);
        this->text = new_text;
        length = new_size;
    }

    void print()
    {

        int printed = 0;
        //printf("printing %i chars, buffer is ", length);
        //printf(text);
        while (printed != length)
        {
            int k = write(1, text + printed, length - printed);
            printed += k;
        }
    }

    void reset()
    {
        length = 0;
        free(text);
    }
};

int main(int argc, char** argv)
{
    int k = 0;
    for (int i = 1; i < argc; i++) 
    {
        if (memcmp(argv[i], ";", 1) == 0){
            break;
        }
        k++;
    }
    char* buffer = (char*)malloc(BUFFER_DEFAULT_SIZE * sizeof(char));
    column_text *texts = (column_text*)malloc(k * sizeof(column_text));
    int* sep_lens = (int*)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++)
    {
        sep_lens[i] = strlen(argv[i + 1]);
    }
    int length = 0;
    int current_column = 0;
    while (true) 
    { 
       int colend = -1;
       int new_read = read(0, buffer + length,  BUFFER_DEFAULT_SIZE - length);
       //printf("DEBUG: current buffer is\n");
       //printf(buffer);
       //printf("\n");
       for (int i = 0; i < length + new_read - sep_lens[current_column]; i++)
       {
         bool flag = true;
         for (int j = 0; j <  sep_lens[current_column]; j++)
         {
           if (buffer[i + j] != argv[current_column + 1][j]) 
           {
             flag = false;
           }
         }
         if (flag)
         {
            colend = i + sep_lens[current_column];
            break;
         }
       }
       if (colend >= 0)
       {
          texts[current_column].update(buffer, colend);
          memmove(buffer, buffer + colend, BUFFER_DEFAULT_SIZE - colend);
          length = BUFFER_DEFAULT_SIZE - colend;
          current_column++;
       } else 
       {
         texts[current_column].update(buffer, BUFFER_DEFAULT_SIZE);
         length = 0;
       }
           //printf("DEBUG: k is %i, argc is %i\n", k, argc);
       //printf("DEBUG: current column is %i, current size is %i\n", current_column, k);
       if (current_column == k)
       {
           //printf("DEBUG: printing line\n");
           for (int i = k + 2; i < argc; i++)
           {
               int num = atoi(argv[i]);
               //printf("num is %i\n", num);
               texts[num].print();
           }
           current_column = 0;
          // write(1, "\n", 1);
           for (int i = 0; i < k; i++)
           {
               texts[i].reset();
           }
       }
       if(new_read == 0 && length == 0) break;
    }
}

