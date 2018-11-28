#!/usr/bin/env python3

"""
    add pragma message to check include structure, only for debug
"""

import os
import fnmatch
import re
import sys


file_included_as_source_files = []

# some file include source file, so check #include statement in the beginning
def check_include_files(filecontents):
    #delete comments first
    lines = []
    flag = 0
    quote = 0
 
    for line in filecontents:
        myline = ''
        length = len(line)
        for index in range(length):
            if flag == 0 and quote == 0 and line[index] == '"':
                quote = 1
                myline += line[index] 
                continue
            if flag == 0 and quote == 1 and line[index] == '"':
                quote = 0
                myline += line[index]
                continue
            if quote != 1 and flag == 2 and line[index] == '\n':
                flag = 0
            if quote != 1 and flag == 0 and line[index] == '/' and line[index+1] == '*':
                flag = 1
            if quote != 1 and index > 0 and flag == 1 and line[index-1] == '/' and line[index-2] == '*':
                flag = 0
            if quote != 1 and flag == 0 and line[index] == '/' and line[index+1] == '/':
                flag = 2
            if flag == 1 or flag == 2:
                continue
            myline += line[index]
        if line.strip() != '' and line.strip() != '\n':
            lines.append(line)
    
    #check include file except included source file in structure, check (){}[] sign
    contents = ''.join(lines)
    includefiles = []
    searchresult = re.search(re.compile('#include\s*[<"][^>"<]+[>"]'), contents)
    while searchresult:
        #print(searchresult)
        pos = searchresult.span()
        contents = contents[pos[1]:]
        structuresignmatch = re.search(re.compile('[(){}\[\]]'), contents)
        includefile = re.sub(re.compile('^\s*#include\s*[<"]'), '', searchresult.group(0))
        includefile = re.sub(re.compile('[>"].*'), '', includefile)
        includefile = includefile.strip()
        #print('structuresignmatch=', structuresignmatch)
        if not structuresignmatch:
            includefiles.append(includefile)
        elif not re.match(re.compile('[)}\]]'), structuresignmatch.group(0)):
            try:
                if contents[contents.index('\n')-1] != '\\':
                    includefiles.append(includefile)
            except ValueError:
                includefiles.append(includefile)
        else:
            file_included_as_source_files.append(os.path.basename(includefile))
        searchresult = re.search(re.compile('#include\s*[<"][^>"<]+[>"]'), contents)
    return includefiles


def add_pragma_message_in_file(file):
    if os.path.basename(file) in file_included_as_source_files:
        return

    contents = []
    filecontents = []

    with open(file) as fd:
        filecontents = fd.readlines()

    includefiles = check_include_files(filecontents)

    contents = []
    for line in filecontents:
        if not re.match(re.compile('^\s*#include\s*[<"][^<>"]+[>"].*'), line):
            contents.append(line)
            continue
        # include statement
        includefile = re.sub(re.compile('^\s*#include\s*[<"]'), '', line)
        includefile = re.sub(re.compile('[>"].*'), '', includefile)
        includefile = includefile.strip()
        #print(line)
        #print(includefile)
        contents.append(line)
        if includefile in includefiles:
            contents.append('#pragma message("{0} is included in {1}")\n'.format(includefile, file))

    with open(file, 'w') as outfd:
        outfd.write(''.join(contents))

def add_pragma_message_in_dir_files(directory):
    directory = os.path.abspath(directory)
    for path, subdirs, files in os.walk(directory):
        for file in files:
            if fnmatch.fnmatch(file, '*.c') or fnmatch.fnmatch(file, '*.h') or fnmatch.fnmatch(file, '*.cpp'):
                #print(os.path.join(path, file))
                add_pragma_message_in_file(os.path.join(path, file))

if __name__ == '__main__':
    file_or_dir = sys.argv[1]
    #add_pragma_message_in_files(file_or_dir)
    #check_include_files(file_or_dir)
    add_pragma_message_in_dir_files(file_or_dir)