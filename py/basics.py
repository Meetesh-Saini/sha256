# Functions required for manipulating bits

def shr(n, r):
    return n >> r


def rot(n, r):
    return (n >> r) | (n << (32 - r)) & 0xFFFFFFFF


def add(a, b):
    return (a+b) % 2**32


def sigma0(n):
    return rot(n, 7) ^ rot(n, 18) ^ shr(n, 3)


def sigama1(n):
    return rot(n, 17) ^ rot(n, 19) ^ shr(n, 10)


def SIGMA0(n):
    return rot(n, 2) ^ rot(n, 13) ^ rot(n, 22)


def SIGMA1(n):
    return rot(n, 6) ^ rot(n, 11) ^ rot(n, 25)


def choice(a, b, c):
    return (a & b) | ((a ^ 0xFFFFFFFF) & c)


def majority(a, b, c):
    return (a & b) | (b & c) | (a & c)
