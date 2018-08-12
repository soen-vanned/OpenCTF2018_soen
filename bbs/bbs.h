#ifndef BBS
#define BBS

#define BAD_USER_ID -1
#define USER_COUNT 4
#define ROOT_USER 3
#define TERRANCETHETENTS 2
#define USERNAME_SIZE 0x10
#define PASSWORD_SIZE 0x10
#define TEXT_SIZE 0x100
#define DEP_PASSWORD_SIZE 0x11

#define MENU_PRINT 0
#define MENU_READ_ALL 1
#define MENU_READ_SINGLE 2
#define MENU_POST 3
#define MENU_CHANGE_PASSWORD 4
#define MENU_CHANGE_USERNAME 5
#define MENU_EXTERNAL_SERVICES 6
#define MENU_QUIT 7

#define NUM_POSTS 7
#define POST_SIZE 1024


typedef struct Credential {
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
} Credential;

typedef struct Post {
    char* username;
    char text[POST_SIZE];
} Post;

#endif