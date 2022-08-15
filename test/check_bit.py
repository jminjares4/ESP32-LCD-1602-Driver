from os import system as sys
import time 


def check_high(x):
    i = 0
    val = 0x01
    pins = 4

    # (value >> i) & 0x01

    print(f'Checking {hex(x)}: ')
    for i in range(0,4):
        if( (x >> i) & val):
            print(f'BIT{i}: 1', end=' ')
        else:
            print(f'BIT{i}: 0', end=' ')
    print('\n')

check_high(0x28)
check_high(0x28 >> 4)
check_high(0x08)
check_high(0x01)
check_high(0x06)
check_high(0x0C)
