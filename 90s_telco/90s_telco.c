#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include "90s_telco.h"

/*
build this to be m64, but with fnostackprotector
*/

int main_menu()
{
    char choice[2];
    printf("0. Enter secret\n");
    printf("1. Get privs\n");
    printf("2. Print data\n");
    printf("3. Set username\n");
    printf("4. Pause\n");
    printf("5. Print stats\n");
    printf("6. Run command\n");
    printf("7. Exit\n");
    fflush(stdout);
    read(0, &choice[0], 2);
    choice[1] = 0;
    return atoi(choice);
}

void get_flag()
{
    int fd = open("/home/challenge/90s_telco.flag", O_RDONLY);
    int c = 1;
    char s[2];
    if (fd == -1)
    {
        printf("Cannot open the flag file\n");
        fflush(stdout);
    }
    while (c)
    {
        c = read(fd, &s, 1);
        s[1] = 0;
        strcat(flag, s);
    }
    flag[strlen(flag)] = 0;
    close(fd);
}

void loop(size_t data)
{
    int choice, c, fd, max;
    Page p;
    char tmp[TMP_SIZE];
    p.privs = 0;
    p.data = data;
    while (1)
    {
        switch(main_menu())
        {
            case 0: // enter secret
                printf("Enter Secret\n");
                fflush(stdout);
                bzero(tmp, CMD_SIZE);
                read(0, tmp, 5);
                if (!strcmp("super_secret", tmp))
                {
                    p.privs = 1;
                } else {
                    printf("UNAUTHORIZED!\n");
                    fflush(stdout);
                }
                break;
            case 1: // get privs
                printf("Unauthorized\n");
                fflush(stdout);
                break;
            case 2: // print privs
                printf("Privs: 0x%lx\n", p.privs);
                fflush(stdout);
                break;
            case 3: // set username BUFFER OVERFLOW HERE IF PRIV
                bzero(p.username, 0x20);
                if (p.privs)
                {
                    printf("Enter username: ");
                    fflush(stdout);
                    c = 1;
                    max = 0;
                    read(0, p.username, TMP_SIZES);
                } else {
                    read(0, &p.username[0], 0x1f);
                }
                printf("Thanks!\n");
                fflush(stdout);
                break;
            case 4: // pause
                bzero(tmp, TMP_SIZE);
                while (1)
                {
                    sprintf(tmp, "/tmp/%dtmp%d", rand(), rand());
                    fd = open(tmp, O_WRONLY | O_CREAT, 0777);
                    if (fd != -1)
                    {
                        write(fd, &p, sizeof(Page));
                        close(fd);
                        break;
                    }
                }
                printf("sleeping...\n");
                fflush(stdout);
                sleep(3); // TOCTOU vulnerability
                while (1)
                {
                    fd = open(tmp, O_RDONLY);
                    if (fd != -1)
                    {
                        read(fd, &p, sizeof(Page));
                        close(fd);
                        break;
                    }
                }
                unlink(tmp);
                break;
            case 5: // print stats
                printf("Privs: 0x%lx\n", p.privs);
                printf("data: 0x%lx\n", p.data);
                printf("username '%s'\n", p.username);
                fflush(stdout);
                break;
            case 6: // run command
                bzero(cmd_string, CMD_SIZE);
                read(0, cmd_string, CMD_SIZE);
                printf("Running commands is a security risk!\n");
                fflush(stdout);
                break;
            case 7: // exit
                return;
                break;
            default:
                printf("Invalid\n");
                fflush(stdout);
                break;
        }
    }
}

int main(int argc, char** argv, char **envp)
{
    umask(0);
    size_t data = time(0);
    get_flag();
    setuid(1000);
    setgid(1000);
    // bin needs to be suid
    fflush(stdout);
    alarm(30);
    srand(data);
    loop(data);
}
