#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INP_SIZE 0x20

// password
// 0123456789abcdef0123456789abcdef
// ACXXXXASLKDH01kSDJ0asdkjFF29slas
//    ^--X == time

int main(int argc, char** argv, char **envp)
{
    alarm(30);
    char pass[INP_SIZE];
    unsigned int t = time(0);
    printf("%d\n", t);
    fflush(stdout);
    if (INP_SIZE == read(0, pass, INP_SIZE))
        if (pass[0] == 'A')  //A
            if ((pass[1] ^ pass[0]) == 2) //C
                if (*(unsigned int *)&pass[2] == t)  //XXXX - time
                    if (pass[6] == 'A')  // A
                        if (pass[7] == 'S')  // S
                            if ((pass[8] ^ pass[0]) == 0xd)  // L
                                if ((pass[9] ^ pass[0]) == 0xa)  // K
                                    if ((pass[0xa] ^ pass[0]) == 0x5)  // D
                                        if ((pass[0xb] ^ pass[0]) == 0x9)  // H
                                            if ((pass[0xc] ^ pass[0]) == 0x71)  // 0
                                                if ((pass[0xd] ^ pass[0]) == 0x70)  // 1
                                                    if ((pass[0xe] ^ pass[0]) == 0x2a)  // k
                                                        if ((pass[0xf] ^ pass[0]) == 0x12)  // S
                                                            if ((pass[0x10] ^ 0x47) == 0x3)  // D
                                                                if ((pass[0x11] ^ 0x88) == 0xc2)  // J
                                                                    if ((pass[0x12] ^ 0x25) == 0x15)  // 0
                                                                        if ((pass[0x13] ^ 0xb9) == 0xd8)  // a
                                                                            if ((pass[0x14] ^ 0x28) == 0x5b)  // s
                                                                                if ((pass[0x15] ^ 0x5b) == 0x3f)  // d
                                                                                    if ((pass[0x16] ^ 0x83) == 0xe8)  // k
                                                                                        if ((pass[0x17] ^ 0xf1) == 0x9b)  // j
                                                                                            if ((pass[0x18] ^ 0xba) == 0xfc)  // F
                                                                                                if ((pass[0x19] ^ 0xd) == 0x4b)  // F
                                                                                                    if ((pass[0x1a] ^ 0x13) == 0x21)  // 2
                                                                                                        if ((pass[0x1b] ^ 0x1b) == 0x22)  // 9
                                                                                                            if ((pass[0x1c] ^ 0xc4) == 0xb7)  // s
                                                                                                                if (pass[0x1d] == 'l')  // l
                                                                                                                    if (pass[0x1e] == 'a')  // a
                                                                                                                        if (pass[0x1f] == 's')  // s
                                                                                                                        {
                                                                                                                            printf("You win!\n");
                                                                                                                            system("/bin/sh");
                                                                                                                        }
    return 0;            
}
