#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "itanic.h"
#include "sha256.h"
#include "aes.h"

FileFormat prow;
void **kpp_addr = 0;
char *itanic;
CPU cpu;

void emulation(void)
{
    int i, counter, tmp, debug=1, sz;
    FileFormat* ff;
    printf("\t[ Casting off the anchor ]\n\n\n");
    fflush(0);
    while (cpu.pc < cpu.end_instruction)
    {
        char op = itanic[cpu.pc];
	/*printf("Running %c\n", op);
    	printf("private_key 0x%lx\n", cpu.private_key);
	printf("pc 0x%lx\n", cpu.pc);
	printf("dp 0x%lx\n", cpu.dp);
	printf("e_start 0x%lx\n", cpu.e_start);
	printf("e_end 0x%lx\n", cpu.e_end);
	printf("end_instruction 0x%lx\n", cpu.end_instruction);
	printf("*pc 0x%x\n", itanic[cpu.pc]);
	printf("*dp 0x%x\n", itanic[cpu.dp]);*/
        switch(op)
        {
            case '>':
                cpu.dp++;
                if (debug)
                {
                    if (cpu.dp > (MAX_MEMORY - 1))
                    {
                        printf("Going over\n");
                        fflush(0);
                        cpu.dp = 0;
                    }
                }
                break;
            case '<':
                cpu.dp--;
                if (debug)
                {
                    if ((signed long int)cpu.dp < 0)
                    {
                        printf("Going under\n");
                        fflush(0);
                        cpu.dp = MAX_MEMORY - 1;
                    }
                }
                break;
            case '+':
                itanic[cpu.dp] = itanic[cpu.dp] + 1;
                break;
            case '-':
                itanic[cpu.dp] = itanic[cpu.dp] - 1;
                break;
            case ',':
                fflush(0);
                read(0, &itanic[cpu.dp], 1);
                break;
            case '.':
                printf("%c", itanic[cpu.dp]);
                fflush(0);
                break;
            case '[':
                counter = 0;
                if (!itanic[cpu.dp])
                {
                    for (i=cpu.pc; i < cpu.end_instruction; i++)
                    {
                        if (itanic[i] == ']')
                            counter--;
                        if (itanic[i] == '[')
                            counter++;
                        if (!counter)
                        {
                            cpu.pc = i;
                            break;
                        }
                    }
                }
                break;
            case ']':
                counter = 0;
                if (itanic[cpu.dp])
                {
                    for (i=cpu.pc; i > 0; i--)
                    {
                        // printf("counter == %d\n", counter);
                        if (itanic[i] == '[')
                            counter--;
                        if (itanic[i] == ']')
                            counter++;
                        // printf("counter == %d\n", counter);
                        if (!counter)
                        {
                            cpu.pc = i;
                            break;
                        }
                    }
                }
                break;
            case '?':
                cpu.private_key = *(uint64_t *)(&itanic[cpu.pc + 1]);
                *(uint64_t *)(&itanic[cpu.pc + 1]) = 0;
                //printf("private key loaded: 0x%lx\n", cpu.private_key);
                cpu.pc += 8;
                break;
            case '#': // Load the size of the encrypted area into the CPU
                cpu.e_size = *(uint64_t *)(&itanic[cpu.pc + 1]);
                *(uint64_t *)(&itanic[cpu.pc + 1]) = 0;
                //printf("encryted size loaded: 0x%lx\n", cpu.e_size);
                cpu.pc += 8;
                break;
            case '*': // Decrypt memory
                decrypt_memory();
                break;
            case '^': // debug_mode
                debug = 0;
                printf("The ship is crawling with vermin, releasing cats!\n");
                fflush(0);
                break;
            case '%':
                kpp(&itanic[0], &itanic[0] + MAX_MEMORY);
                break;                
            case '!':
                printf("\nCustom program loader!\n");
                fflush(0);
                read(0, &prow, sizeof(FileFormat));
                //printf("prow: 0x%x\n", prow.header);
                if (prow.header != 0x54414f42)
                {
                    printf("This is not a boat!");
                    exit(-1);
                }
                free(itanic);
                itanic = malloc(MAX_MEMORY);
                if (itanic)
                {
                    bzero(&itanic[0], MAX_MEMORY);
                } else {
                    printf("Captain's drunk!\n");
                    exit(-1);
                }
                sz = 0;
                while (sz < prow.program_size)
                {
                    sz += read(0, &itanic[sz], 1);
                }
                amfid_v2((CodeBlob *)itanic);
                cpu.pc--;
                break;
        }
    cpu.pc++;
    }
    printf("\n\n\t[ The Itanic still floats! ]\n\n");
}

int main(int argc, char** argv, char **envp)
{
    fflush(0);
    if (!(argc == 2) && (strcmp(argv[2], "LostKng.enc")))
    {
        alarm(600);
    } else {
        alarm(300);
    }
    int ret = -1;
    float version = 1.005003;
    printf("\t[ Itanic v %f ]\n", version);
    if (argc != 2)
    {
    printf("Welcome to HMS Operating System\n");
	printf("Written on a custom (ish) architecture.\n");
	printf("This operating system is perfectly\n");
	printf("secure (unhackable like BitFi).\n");
	printf("\n");
	printf("Using the Itanic processor, the superior\n");
	printf("processor for the unsinkable HMS\n");
	printf("operating system.\n");
	printf("\n");
	printf("\n");
	printf("Main menu\n");

	printf("1. Printer\n");
	printf("2. Hello_world\n");
	printf("3. Dock\n");
	printf("4. Quine\n");
    printf("5. Lost Kingdom (not part of challenge, but relaxing and fun!)\n");
	printf("6. quit\n");
        fflush(0);

	while(1)
	{
		char choice[2];
		read(0, &choice, 2);
		char* arr[3];
		arr[0] = argv[0];
        arr[2] = 0;
        printf("[%c]\n", choice[0]);
		switch(choice[0])
		{
		    case '1':
			arr[1] ="/home/challenge/printer.enc";
			execve(arr[0], arr, envp); 
			break;
		    case '2':
			arr[1] = "/home/challenge/hello_world.enc";
			execve(arr[0], arr, envp); 
			break;
		    case '3':
			arr[1] = "/home/challenge/dock.enc";
			execve(arr[0], arr, envp); 
			break;
		    case '4':
			arr[1] = "/home/challenge/quine.enc";
			execve(arr[0], arr, envp); 
			break;
                    case '5':
                        arr[1] = "/home/challenge/LostKng.enc";
			execve(arr[0], arr, envp); 
		    case '6':
			exit(0);
		}
	}
    } else {
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0)
        {
            printf("Problem getting into the boat\n");
            fflush(0);
            return ret;
        } else {
            int tmp = read(fd, &prow, sizeof(FileFormat));
            //printf("Read in 0x%x vs 0x%lx\n", tmp, sizeof(FileFormat));
            if (tmp == sizeof(FileFormat))
            {
                if (prow.header != 0x54414f42)
                {
                    printf("This is not a boat!\n");
                    fflush(0);
                    return ret;
                }
                // printf("0x%x\n", prow.header);
                // printf("program_size 0x%llx\n", prow.program_size);
                cpu.mem_size = MAX_MEMORY;
                itanic = (char *)malloc(MAX_MEMORY);
                bzero(itanic, MAX_MEMORY);
                if (itanic)
                {
                    // printf("prow.program_size == 0x%llx\n", prow.program_size);
                    uint64_t sz = read(fd, (void *)itanic, prow.program_size);
                    // printf("sz = 0x%llx\n", sz);
                    if (sz == prow.program_size)
                    {
                        //mem = (char *)malloc(cpu.mem_size);
                        if (1)
                        {
                            //bzero(mem, cpu.mem_size);
                            amfid_v2((CodeBlob *)itanic);
                            emulation();
                            ret = 0;
                        } else {
                            printf("Air is too cloudy to sail\n");
                        }
                    } else {
                        printf("Punched holes in boat trying to enter it!\n");
                    }
                } else {
                    printf("Ocean not big enough for boat\n");
                }
            } else {
                printf("Holes in the boat!\n");
            }
            close(fd);
            fflush(0);
        }
    }
    return ret;
}
/*
typedef struct CodeBlob
{
    char key[0x20];
    char iv[0x10];
    uint64_t code_size;
    char code[1];
}

*/



extern int aes_decrypt_cbc(const BYTE in[], size_t in_len, BYTE out[], const WORD key[], int keysize, const BYTE iv[]);
extern int aes_encrypt_cbc(const BYTE in[], size_t in_len, BYTE out[], const WORD key[], int keysize, const BYTE iv[]);

void amfid_v2(CodeBlob* blob)
{
    int i;
    /*
        signature
        blob:
            key
            iv
            size
            encrypted_data

    */

    WORD key_schedule[60];
    for (i=0;i<60;i++)
        key_schedule[i] = 0;
     // printf("About to malloc blob->code_size (0x%lx)\n", blob->code_size);
    char *buf = malloc(blob->code_size);
    if (!buf)
    {
        printf("Comparments flooding! Impossible! 0x%lx\n", blob->code_size);
    }
     // printf("buf == %p\n", buf);
    aes_key_setup((BYTE *)blob->key, key_schedule, 256);
     // for (i=0;i<0x20;i++)
     //    printf("%02x", blob->key[i] & 0xff);
     // printf("\nIV: ");
     // for (i=0;i<0x10;i++)
     //    printf("%02x", blob->iv[i] & 0xff);
    aes_decrypt_cbc((BYTE *)blob->code, blob->code_size, (BYTE *)buf, key_schedule, 256, (BYTE *)blob->iv);
     // printf("\nCode size==0x%lx", blob->code_size);
     // printf("\nDecrypted \"");
    
     // for (i=0; i<blob->code_size; i++)
     //     printf("%c", buf[i]);
     // printf("\"\n");

/*
typedef struct CodeBlob
{
    char signature[0x20];
    char key[0x20];
    char iv[0x10];
    uint64_t code_size;
    char code[1];
} CodeBlob;
*/

    uint64_t header = sizeof(CodeBlob)-0x20;
    uint64_t code_size_m = prow.program_size-0x20;
    // printf("blob_size = 0x%lx\n", code_size_m);
    void *csc = malloc(code_size_m);
    memcpy(csc, (void *)(blob->key), header);
    memcpy((void*)(csc + header - 8), &buf[0], blob->code_size);


    // printf("\n");

    // for (i=0;i<code_size_m;i++)
    // {
    //     if (!(i % 16))
    //         printf("\n");
    //     printf("%02x ", *(char *)(csc+i) & 0xff);
    // }
    // printf("\n");

    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, (BYTE *)csc, code_size_m);
    sha256_final(&ctx, hash);
    free(csc);
    // printf("blob sha256 is 0x");
    // for (i=0; i<0x20; i++)
    //     printf("%02x", hash[i] & 0xff);

    // printf("\n");

    if (!memcmp(hash, blob->signature, 0x20))
    {
        printf("\t[ Your sextant is accurate! ]\n");
    } else {
        printf("\t[ The tides are against you! ]\n");
        printf("\t[ Better sink myself ]\n");
        exit(-1);
    }

    //copy buf back into cpu space
    cpu.pc = 0;
    cpu.dp = blob->code_size;
    cpu.e_start = 0;
    cpu.e_end = 0;
    cpu.e_size = 0;
    cpu.target = 0;
    cpu.end_instruction = blob->code_size;
    cpu.running = 1;
    memcpy(itanic, buf, blob->code_size);
    blob = 0;
    bzero(itanic+cpu.end_instruction, MAX_MEMORY-cpu.end_instruction);
    free(buf);
}


void decrypt_memory()
{
    // printf("Decrypt_memory called!\n");
    int i;
    cpu.e_start = cpu.pc + 1;
    cpu.e_end = cpu.e_start + cpu.e_size;
    for (i=cpu.e_start;i<cpu.e_end; i+=8)
    {
        *(uint64_t *)&itanic[i] = *(uint64_t *)&itanic[i] ^ cpu.private_key;
    }
    // printf("Decryption finished!\n");
    // printf("%s\n", &itanic[cpu.e_start]);
}

void kpp_validation(void* start_addr, void* q1, void* q2, void* end_addr)
{
    printf("KPP unimplemented\n");
}

void kpp(void* start_addr, void* end_addr)
{
    if (!kpp_addr)
    {
        kpp_addr = malloc(sizeof(void*)*0x100);
        kpp_addr[0] = kpp_validation;
        kpp_addr[1] = start_addr;
        kpp_addr[2] = start_addr + MAX_MEMORY / 4;
        kpp_addr[3] = start_addr + MAX_MEMORY / 2;
        kpp_addr[4] = start_addr + MAX_MEMORY / 2 + MAX_MEMORY / 4;
        kpp_addr[5] = end_addr;
    }
   ((void (*)())*kpp_addr)(kpp_addr[1], kpp_addr[2], kpp_addr[3], kpp_addr[4], kpp_addr[5]);
}
