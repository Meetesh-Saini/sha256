// Functions required for bit manipulation

unsigned long shr(unsigned long n, unsigned long r)
{
    return n >> r;
}

unsigned long rot(unsigned long n, unsigned long r)
{
    return (n >> r) | (n << (32 - r)) & 0xFFFFFFFF;
}

unsigned long sigma0(unsigned long n)
{
    return rot(n, 7) ^ rot(n, 18) ^ shr(n, 3);
}

unsigned long sigama1(unsigned long n)
{
    return rot(n, 17) ^ rot(n, 19) ^ shr(n, 10);
}

unsigned long SIGMA0(unsigned long n)
{
    return rot(n, 2) ^ rot(n, 13) ^ rot(n, 22);
}

unsigned long SIGMA1(unsigned long n)
{
    return rot(n, 6) ^ rot(n, 11) ^ rot(n, 25);
}

unsigned long choice(unsigned long a, unsigned long b, unsigned long c)
{
    return (a & b) | ((a ^ 0xFFFFFFFF) & c);
}

unsigned long majority(unsigned long a, unsigned long b, unsigned long c)
{
    return (a & b) | (b & c) | (a & c);
}