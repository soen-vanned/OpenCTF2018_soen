#ifndef ITANIC_HEADER
#define ITANIC_HEADER

#define EMULATION_FINISHED 1

typedef struct CPU
{
    uint64_t private_key;
    uint64_t pc;
    uint64_t dp;
    uint64_t e_start;
    uint64_t e_end;
    uint64_t e_size;
    uint64_t target;
    uint64_t end_instruction; 
    uint64_t mem_size;  
    uint64_t running;
} CPU;

typedef struct FileFormat
{
    uint32_t header;
    uint32_t version;
    uint64_t program_size;
} FileFormat;

typedef struct TrustCache
{
    void * signature[0x10];
} TrustCache;

typedef struct CodeBlob
{
    char signature[0x20];
    char key[0x20];
    char iv[0x10];
    uint64_t code_size;
    char code[1];
} CodeBlob;

void emulation(void);
void amfid_v2(CodeBlob* blob);
void decrypt_memory(void);
void kpp(void* start_addr, void* end_addr);
void checker(void);

#define MAX_MEMORY 0x400000

#endif


/*
signature
    blog:
        key
        iv
        size
        encrypted_data
*/
