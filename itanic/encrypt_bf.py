#!/usr/bin/env python

from sys import argv
from random import randint
from pwn import *

if len(argv) != 3:
    print 'Usage\n\t%s <source bf> <output encrypted>' % argv[0]
    exit(0)

bf = open(argv[1]).read()
key = randint(0, 0xffffffffffffffff)
enc = ''
bf += '\0' * (8 - len(bf) % 8)
for i in range(0, len(bf), 8):
    enc += p64(u64(bf[i:i+8]) ^ key)

print 'key: 0x%x' % key
print 'len: 0x%x' % len(bf)

data  = '?' + p64(key)
data += '#' + p64(len(bf))
data += '*'
data += enc

open(argv[2], 'wb').write(data)
