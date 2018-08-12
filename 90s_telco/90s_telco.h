#include <stdint.h>
#include <sys/types.h>

#ifndef NTS
#define NTS

#define TMP_SIZE 0x40
#define TMP_SIZES 0x80
#define CMD_SIZE 0x500

typedef struct Page
{
    uint64_t privs;
    uint64_t data;
    char username[0x20];
} Page;

char flag[TMP_SIZE];
char cmd_string[CMD_SIZE];

int main_menu(void);
void get_flag(void);
void loop(size_t data);

#endif
