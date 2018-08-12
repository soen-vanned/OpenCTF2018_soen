#!/usr/bin/env python

from pwn import *
from sys import argv
from random import randint
from Crypto.Cipher import AES
from Crypto import Random
import hashlib

def dump_blob(blob):
    i = 0
    ret = ''
    while i < len(blob):
        ret += '%02x ' % ord(blob[i])
        i += 1
        if not (i % 16):
            print ret
            ret =''
    if ret:
        print ret

def encrypt_and_sign(data, out_file, key):
    data += '\0'*(16 - len(data) % 16)
    '''
        typedef struct CodeBlob
        {
            char key[0x20];
            char iv[0x10];
            uint64_t code_size;
            char code[1];
        } CodeBlob;
    '''
    blob = key # 1
    
    iv = Random.new().read(AES.block_size)
    blob += iv # 2
    print 'iv is',
    tmp = ''
    for i in iv:
        tmp += '%02x' % ord(i)
    print tmp

    m = AES.new(key, AES.MODE_CBC, iv)

    enc = m.encrypt(data)
    enc_data_length = len(enc)
    print 'enc_data_length, 0x%x' % enc_data_length
    blob += p64(enc_data_length) # 3
    blob += enc # 4
    #print enc

    ublob = key + iv + p64(enc_data_length) + data
    sig = hashlib.sha256()
    print 'blob_size 0x%x' % len(ublob)
    #dump_blob(ublob)
    sig.update(ublob)
    digest = sig.digest()
    print 'blob sha256 is 0x%s' % sig.hexdigest()

    data = digest + blob
    print 'length of data 0x%x' % len(data)
    file_header = 'BOAT' + p32(2) + p64(len(data))

    # write it to file
    out_file.write(file_header + data)
    out_file.close()

    '''
    signature
    blob:
        key
        iv
        size
        encrypted_data
    '''

if __name__=='__main__':
    if len(argv) < 3:
        print 'Usage:\n\t%s <code file to encrypt and sign> <output file>' % argv[0]
        exit(-1)

    in_file_data = open(argv[1]).read()
    out_file = open(argv[2], 'wb')

    key1 = randint(0,0xffffffffffffffff)
    key2 = randint(0,0xffffffffffffffff)
    key3 = randint(0,0xffffffffffffffff)
    key4 = randint(0,0xffffffffffffffff)

    print 'encrypting & signing with key:\n\t0x%x\n\t0x%x\n\t0x%x\n\t0x%x' % (key1, key2, key3, key4)
    key = p64(key1, endianness='little') +\
          p64(key2, endianness='little') +\
          p64(key3, endianness='little') +\
          p64(key4, endianness='little')

    encrypt_and_sign(in_file_data, out_file, key)
