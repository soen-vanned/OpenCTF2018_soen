#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include "bbs.h"

/*
business logic bug to get the other user
off by one to leak the root user password
reading a vulnerable message by the root user actually triggers the bug

GUEST GUEST
BORKIN BORKINPW
TERRANCETHETENT KALSDJL981ASDKLJ
ROOT 981hod198skla

MAKE THE PASSWORDS RANDOM OMG



rop to win this?

*/

void setup(void);
void header(void);
void login(void);
void bbs(void);
void menu(void);
void change_password(void);
void change_username(void);
void read_single(void);
void do_post_read(int post_id);
void external_services(void);
void write_post(void);

Credential creds[USER_COUNT];
char *ls_cmd = "ls -la";
char *welcome = "ARM 400 BBS\n";
Post posts[NUM_POSTS];

char pw[PASSWORD_SIZE+1];
char user_aa_name[USERNAME_SIZE];
int user_id_aa;



void write_post(void)
{
    printf("What is your post content?\n");
    fflush(stdout);
    posts[6].username = strdup(creds[user_id_aa].username);
    read(0, posts[6].text, 0x400);
}

void external_services(void)
{
    char banner[32];
    printf("Here's a directory listing for diagnostic purposes:\n");
    fflush(stdout);
    system(ls_cmd);
    header();
    printf("Change banner: \n");
    fflush(stdout);
    // bug code execution
    printf("Read in %d bytes\n", read(0, banner, 64)); // pair this down to have just enough gadgets
    printf("%s\n", strerror(errno));
}

void do_post_readall()
{
    int i;
    for (i=0;i<NUM_POSTS;i++)
    {
        printf("Post by: %s\n", posts[i].username);
        printf("Post content: %s\n", posts[i].text);
    }
    fflush(stdout);
}

void do_post_read(int post_id)
{
    if (post_id > 0)
    {
        int i;
        for (i=0;i<NUM_POSTS;i++)
        {
            printf("Post by: %s\n", posts[i].username);
            printf("Post content: %s\n", posts[i].text);
        }
    }
    fflush(stdout);
}

void read_single()
{
    int i;
    char post_number[0x40];
    printf("Enter post number to read: ");
    fflush(stdout);
    read(0, post_number, 0x40);
    for(i=0;i<0x40;i++)
        if (post_number[i] == 'n')
        {
            printf("n is for NO\n");
            fflush(stdout);
            exit(-1);
        }
    printf("Post #");
    printf(post_number);
    printf("\n");
    fflush(stdout);
    do_post_read(atoi(post_number));
}

void change_username()
{
    int i;
    for (i=0; i<USER_COUNT; i++)
        if (!strncmp(pw, creds[i].password, PASSWORD_SIZE))
            printf("Your old username is %s\n ", creds[i].username);
    printf("Enter your new username: ");
    fflush(stdout);
    // bug, allows you to go from GUEST to BORKIN
    scanf("%17s", user_aa_name);
    // VALIDATE THAT YOU CAN'T JUST CHANGE YOUR USERNAME TO ROOT
    for (i=0; i<USER_COUNT; i++)
        if (!strncmp(pw, creds[i].password, PASSWORD_SIZE))
            strncpy(creds[i].username, user_aa_name, USERNAME_SIZE);
    user_id_aa &= 1; //  but not to root!!!
    //printf("Your user ID is %d\n", user_id_aa);
}

void change_password()
{
    char* pass = creds[user_id_aa].password;
    printf("%s old password is %s\n ", creds[user_id_aa].username, pass);
    printf("Enter your new password: ");
    fflush(stdout);
    // bug, allows you to go from BORKIN to TERRANCETHETENTS but not from guest to BORKIN
    scanf("%17s", pw);
    strncpy(pass, pw, strlen(pw));
    printf("Your new password is %s\n", creds[user_id_aa].password);
}

void bbs()
{
    int choice = MENU_QUIT;
    menu();
    while (1)
    {
        printf("&--> ");
        fflush(stdout);
        fflush(stdin);
        choice = -1;
        scanf("%d", &choice);
        switch (choice)
        {
            case MENU_READ_ALL:
                do_post_readall();
                break;
            case MENU_READ_SINGLE:
            if ((user_id_aa == TERRANCETHETENTS) || (user_id_aa == ROOT_USER))
                {
                    read_single();
                }
                break;
            case MENU_POST:
                if (user_id_aa == ROOT_USER)
                {
                    write_post();
                } else {
                    printf("Due to recent abuse, only ROOT is allowed to write posts\n");
                }
                break;
            case MENU_CHANGE_PASSWORD:
                change_password();
                break;
            case MENU_CHANGE_USERNAME:
                change_username();
                break;
            case MENU_QUIT:
                return;
            case MENU_PRINT:
                menu();
                break;
            case MENU_EXTERNAL_SERVICES:
                if (user_id_aa == ROOT_USER)
                {
                    external_services();
                }
                break;
            default:
                printf("Bad option\n");
                sleep(1);
                break;
        }
    }
}

void menu()
{
    printf("ARM400BBS Menu:\n");
    printf("%d - print menu\n", MENU_PRINT);
    printf("%d - read all messages\n", MENU_READ_ALL);
    if ((user_id_aa == TERRANCETHETENTS) || (user_id_aa == ROOT_USER))
        printf("%d - read message #\n", MENU_READ_SINGLE);
    printf("%d - post a message\n", MENU_POST);
    printf("%d - change password\n", MENU_CHANGE_PASSWORD);
    printf("%d - change username\n", MENU_CHANGE_USERNAME);
    if (user_id_aa == ROOT_USER)
        printf("%d - external services\n", MENU_EXTERNAL_SERVICES);
    printf("%d - log out\n", MENU_QUIT);
    fflush(stdout);
}

void setup()
{
    int i = 0;
    int fd = open("/home/challenge/passwords", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening passwords file!!!\n");
        fflush(stdout);
        exit(-1);
    }
    bzero(creds, (USER_COUNT * sizeof(Credential)));
    for (i=0; i < USER_COUNT; i++)
    {
        read(fd,creds[i].username, USERNAME_SIZE);
        read(fd,creds[i].password, PASSWORD_SIZE);
    }
    close(fd);
    posts[0].username = "ROOT";
    strcpy(posts[0].text, "Welcome to the system! We are glad to have you here, please abide by the rules and have fun!");

    posts[1].username = "BORKIN";
    strcpy(posts[1].text, "What up l3wz3rz, Ir sup3r 13373 h4XxX0r.");

    posts[2].username = "TERRANCETHETENTS";
    strcpy(posts[2].text, "Shut up BORKIN!");

    posts[3].username = "ROOT";
    strcpy(posts[3].text, "TERRANCETHETENTS, Please stop hacking the system, I closed up the bug you used to get root");

    posts[4].username = "BORKIN";
    strcpy(posts[4].text, "ⓦⓗⓔⓡⓔ ⓘⓢ ⓣⓗⓔ ⓛⓔⓦⓓⓢ ⓝⓔⓦⓓⓢ ⓞⓡ ⓘⓢ ⓙⓤⓢⓣ ⒷⒷⓈ ⓕⓤⓛⓛ ⓞⓕ ⓓⓤⓓⓒⓔⓢ");

    posts[5].username = "ROOT";
    strcpy(posts[5].text, "BORKIN this is your last warning, you will be kicked from the system.");

    posts[6].username = 0;
    bzero(posts[6].text, POST_SIZE);
}

void login()
{
    
    int i;
    bzero(user_aa_name, USERNAME_SIZE);
    bzero(pw, PASSWORD_SIZE);
    fflush(stdout);
    printf("Enter your username: ");
    fflush(stdout);
    scanf("%16s", user_aa_name);
    printf("Enter your password: ");
    fflush(stdout);
    scanf("%16s", pw);
    user_id_aa = BAD_USER_ID;
    for (i=0; i<USER_COUNT; i++)
    {
        // printf("%s == %s\n", creds[i].username, user_aa_name);
        // printf("%s == %s\n", creds[i].password, pw);
        // fflush(stdout);
        if (!strncmp(user_aa_name, creds[i].username, USERNAME_SIZE))
            if (!strncmp(pw, creds[i].password, PASSWORD_SIZE))
            {
                user_id_aa = i;
                break;
            }
    }
}

void header()
{
    printf("\033[36;1m █████╗ ██████╗ ███╗   ███╗   \033[32;1m ██╗  ██╗ ██████╗  ██████╗    \033[35;1m ██████╗ ██████╗ ███████╗\n");
    printf("\033[36;1m██╔══██╗██╔══██╗████╗ ████║   \033[32;1m ██║  ██║██╔═████╗██╔═████╗   \033[35;1m ██╔══██╗██╔══██╗██╔════╝\n");
    printf("\033[36;1m███████║██████╔╝██╔████╔██║   \033[32;1m ███████║██║██╔██║██║██╔██║   \033[35;1m ██████╔╝██████╔╝███████╗\n");
    printf("\033[36;1m██╔══██║██╔══██╗██║╚██╔╝██║   \033[32;1m ╚════██║████╔╝██║████╔╝██║   \033[35;1m ██╔══██╗██╔══██╗╚════██║\n");
    printf("\033[36;1m██║  ██║██║  ██║██║ ╚═╝ ██║   \033[32;1m      ██║╚██████╔╝╚██████╔╝   \033[35;1m ██████╔╝██████╔╝███████║\n");
    printf("\033[36;1m╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝   \033[32;1m      ╚═╝ ╚═════╝  ╚═════╝    \033[35;1m ╚═════╝ ╚═════╝ ╚══════╝\n");
    printf(welcome);
    fflush(stdout);
}

int main(int argc, char **argv, char**envp)
{
    alarm(30);
    setup();
    login();
    //printf("\033[37;1m");
    printf("User account level %d\n", user_id_aa);
    fflush(stdout);
    if (user_id_aa > BAD_USER_ID)
    {
        header();
        bbs();
    } else {
        printf("Guests are welcome to log in with GUEST | GUEST\n");
    }
    //printf("\033[37;1m");
    fflush(stdout);
    return 0;
}
