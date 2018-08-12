#!/usr/bin/env python
import sys
import signal
import random
from base64 import b64encode, b64decode

flag = open('/home/challenge/flag').read()

def fail():
    print 'Your decoding was not right'
    exit(1)

def hexxer(s):
    ret = ''
    first = True
    for c in s:
        if first:
            ret += hex(ord(c))[2:]
            first = False
        else:
            ret += ' ' + hex(ord(c))[2:]
    print ret
    print '\n? '
    sys.stdout.flush()
    s2 = raw_input()
    if s2 == s:
        return True
    return False

def binary(s):
    ret = ''
    for c in s:
        tmp = "{0:b}".format(ord(c))
        tmp = '0' * (8 - len(tmp)) + tmp
        ret += tmp
    print ret
    print '\n? '
    sys.stdout.flush()
    s2 = raw_input()
    if s2 == s:
        return True
    return False

def morse_code(s):
    # https://gist.github.com/ebuckley/1842461
    morseAlphabet ={"A": ".-", "B": "-...", "C": "-.-.", "D": "-..", "E": ".", "F": "..-.",
                    "G": "--.", "H": "....", "I": "..", "J": ".---", "K": "-.-", "L": ".-..",
                    "M": "--", "N": "-.", "O": "---", "P": ".--.", "Q": "--.-", "R": ".-.",
                    "S": "...", "T": "-", "U": "..-", "V": "...-", "W": ".--", "X": "-..-",
                    "Y": "-.--", "Z": "--..", " ": "/" }
    inverseMorseAlphabet=dict((v,k) for (k,v) in morseAlphabet.items())
    for i in s:
        print morseAlphabet[i],
    print '\n? '
    sys.stdout.flush()
    s2 = raw_input()
    if s2 == s:
        return True
    return False

def b64(s):
    print b64encode(s)
    print '\n? '
    sys.stdout.flush()
    s2 = raw_input()
    if s == s2:
        return True
    return False

def get_random_string(full=True):
    an = ''
    if full:
        an = 'abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    else:
        an = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    ret = ''
    for i in xrange(0x20):
        ret += an[random.randint(0, len(an) - 1)]
    if full:
        return 'string{%s}' % ret
    else:
        return ret

if __name__ == '__main__':
    signal.alarm(5)

    print 'ACM: Talk to the machine'
    print 'decode each of these formats as fast as possible!'
    print 'if you properly solve each of the 12 challenges, you get the flag!'
    sys.stdout.flush()
    counter = 0
    counter_stop = 20
    while counter < counter_stop:
        if b64(get_random_string()):
            if morse_code(get_random_string(full=False)):
                if binary(get_random_string()):
                    if hexxer(get_random_string()):
                        counter += 1
                    else:
                        fail()
                else:
                        fail()
            else:
                        fail()
        else:
                        fail()
    if counter == counter_stop:
        print 'You win!!!'
        print flag
        sys.stdout.flush()