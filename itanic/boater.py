#!/usr/bin/env python
from sys import argv
from pwn import *

if len(argv) < 3:
    print 'Usage:\n\t%s <brainfuck> <output_boat_file>'
    exit(0)

version = 1
program = open(argv[1]).read()
output = open(argv[2], 'wb')
program_size = len(program)
private_key = 0x4142434445464748
pc = 0
dp = 0
e_start = 0
e_end = 0
running = 1
mem_size = 0x4000
target = 0
cpu = p64(private_key) + p64(pc) +\
      p64(dp) + p64(e_start) +\
      p64(e_end) + + p64(target) +\
      p64(program_size) + p64(mem_size) +\
      p64(running)
header = 'BOAT' + p32(version) + p64(program_size)
output.write(header + cpu + program)
output.close()
'''
typedef struct CPU
{
    uint64_t private_key;
    uint64_t pc;
    uint64_t dp;
    uint64_t e_start;
    uint64_t e_end;
    uint64_t end_instruction;   
    uint64_t running;
} CPU;

typedef struct FileFormat
{
    uint32_t header;
    CPU cpu;
    uint64_t program_size;
} FileFormat ;
'''