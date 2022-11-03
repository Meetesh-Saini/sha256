#!/usr/bin/python3

from sys import argv
from hash import sha256

# sha256.py -f t.txt -o f.txt

def fileWout(args : list):
    fileName = args[args.index("-f")+1]
    outName = args[args.index("-o")+1]

    with open(fileName, "rb") as f:
        t = f.read()
    o = sha256(t).encode()
    with open(outName, "wb") as f:
        f.write(o)
    return 0

def fileO(args : list):
    fileName = args[args.index("-f")+1]
    with open(fileName, "rb") as f:
        t = f.read()
    o = sha256(t)
    print(o)
    return 0

def textWout(args : list):
    outName = args[args.index("-o")+1]
    if args[1] == "-o":
        t = args[3]
    else:
        t = args[1]
    o = sha256(t).encode()
    with open(outName, "wb") as f:
        f.write(o)
    return 0

def textO(args : list):
    t = args[1]
    o = sha256(t)
    print(o)
    return 0

if len(argv) > 1 and "-h" not in argv and "--help" not in argv:
    if "-f" in argv:
        if "-o" in argv:
            fileWout(argv)
        else:
            fileO(argv)
    else:
        if "-o" in argv:
            textWout(argv)
        else:
            textO(argv)
else:
    help = "\
./sha256.py : Tool to find sha256 sum of text or file \n\
\n\
Syntax: \n\
./sha256.py [-fo] [filename|text] \n\
\n\
Usage: \n\
./sha256.py text \n\
./sha256.py text -o outputFile \n\
./sha256.py -f fileName \n\
./sha256.py -f fileName -o outputFile \n\
"
    print(help)