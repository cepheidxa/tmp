#!/usr/bin/env python3

import os
import os.path


def find_vendor_system_compiled_directory():
    directory='./out/target/product/'
    if not os.path.isdir(directory):
        print(directory, 'directory not exist, please compile the project first')
        exit(-1)
    for file in os.listdir(directory):
        if not os.path.isdir(os.path.join(directory, file)):
            continue
        subfiles = os.listdir(os.path.join(directory, file))
        if 'system' in subfiles and 'vendor' in subfiles:
            return os.path.join(directory, file)
        else:
            print('vendor and system director is not found')
            exit(-1)

directory = find_vendor_system_compiled_directory()

with os.popen('find {0}/system {0}/vendor -type f'.format(directory)) as fd:
    files = fd.read()

files = files.split()

files = [os.path.basename(x) for x in files]
files = list(set(files))
files.sort()

zte_vs_file_num = len(files)
zte_vs_filename_max_len = max([len(x) for x in files])

print('#ifndef __ZTE_VENDOR_SYSTEM_FILENAME_H__')
print('#define __ZTE_VENDOR_SYSTEM_FILENAME_H__')
print('/*\n * DO NOT MODIFY.\n *\n * This file was generated by software\n */\n')
print('#define ZTE_VENDOR_SYSTEM_FILE_NUM ', zte_vs_file_num, sep='')
print('#define ZTE_VENDOR_SYSTEM_FILENAME_MAX_LEN ', zte_vs_filename_max_len + 1, sep='')
print('')

print('static char *zte_vendor_system_filename_array[ZTE_VENDOR_SYSTEM_FILE_NUM] = {')
i = 0
for file in files:
    print('\t[{0}] = "{1}",'.format(i, file))
    i += 1
print('};')
print()

print('#endif')
