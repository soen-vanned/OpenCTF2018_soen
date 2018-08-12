#include <string.h>
#include <unistd.h>
#include <stdio.h>

// gcc close_your_eyes_to_see.c -o close_your_eyes_to_see -fno-stack-protector -m32

#define DATA_SIZE 0x40
char data[DATA_SIZE];
char bss_432[0x1000-DATA_SIZE];

void vulnerable()
{
    char not_big_enough[0x20];
    int bytes_read = read(0, data, DATA_SIZE);
    memcpy(not_big_enough, data, bytes_read);
}
int main(int argc, char **argv, char **envp)
{
    char data[0x400];
    printf("You don't need libc young padiwan...\n");
    vulnerable();
}
