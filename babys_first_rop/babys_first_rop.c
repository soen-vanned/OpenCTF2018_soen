#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
/*
gcc babys_first_rop.c -o babys_first_rop -fno-stack-protector
*/

#define GADGET_SIZE 0x30000

char gadgets[GADGET_SIZE];
char user_input[0x100];

void vuln()
{
    char too_small[0x40];
    int bytes_read = read(0, user_input, 0x100);
    memcpy(too_small, user_input, bytes_read);
}

int main(int argc, char **argv, char **envp)
{
    alarm(30);
    int i=0, j=0, index=0;
    while (i < 0x100)
    {
        index = i * 0x300 + j*3;
        gadgets[index+0] = i;
        gadgets[index+1] = j;
        gadgets[index+2] = 0xc3;
        if (j >= 0x100)
        {
            j = 0;
            i++;
        } else {
            j++;
        }
    }
    mprotect((void *)((size_t)gadgets & 0xfffffffffffff000),
             GADGET_SIZE,
             7);
    vuln();
}
