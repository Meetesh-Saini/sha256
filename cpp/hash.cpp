#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "basics.h"

// for command line tool
#include <fstream>

using namespace std;

unsigned long constants[] = {1116352408, 1899447441, 3049323471, 3921009573, 961987163, 1508970993, 2453635748, 2870763221, 3624381080, 310598401, 607225278, 1426881987, 1925078388, 2162078206, 2614888103, 3248222580, 3835390401, 4022224774, 264347078, 604807628, 770255983, 1249150122, 1555081692, 1996064986, 2554220882, 2821834349, 2952996808, 3210313671, 3336571891, 3584528711, 113926993, 338241895, 666307205, 773529912, 1294757372, 1396182291, 1695183700, 1986661051, 2177026350, 2456956037, 2730485921, 2820302411, 3259730800, 3345764771, 3516065817, 3600352804, 4094571909, 275423344, 430227734, 506948616, 659060556, 883997877, 958139571, 1322822218, 1537002063, 1747873779, 1955562222, 2024104815, 2227730452, 2361852424, 2428436474, 2756734187, 3204031479, 3329325298};
vector<unsigned long> initials = {1779033703, 3144134277, 1013904242, 2773480762, 1359893119, 2600822924, 528734635, 1541459225};

string sha256(string text)
{
    string message;
    // converting text to binary message
    for (unsigned long i = 0; i < text.length(); i++)
    {
        message += bitset<8>(text[i]).to_string();
    }

    // padding message
    message += '1';
    unsigned long long len_message = message.length();
    int padding_space = len_message % 512;
    unsigned long padding_div = len_message / 512;
    string padded_message = message;
    size_t padding_bits;

    if (padding_space > 448)
    {
        padding_bits = (padding_div + 1) * 512 + 448 - len_message;
        padded_message = message + string(padding_bits, '0');
    }
    else if (padding_space < 448)
    {
        padding_bits = padding_div * 512 + 448 - len_message;
        padded_message = message + string(padding_bits, '0');
    }

    // padding with length
    padded_message += bitset<64>(len_message - 1).to_string();

    // message blocks
    unsigned int block_number = 0;
    unsigned int total_blocks = padded_message.length() / 512;

    // initial hash value
    vector<unsigned long> hash_initials = initials;
    vector<unsigned long> buffer_values;
    unsigned long schedule_list[64];
    unsigned long message_schedule;
    unsigned long t1, t2;
    while (block_number < total_blocks)
    {
        unsigned int start_index = block_number * 512;

        // message block to hash
        string block = padded_message.substr(start_index, 512);

        // buffers:
        // a b c d e f g h
        // 0 1 2 3 4 5 6 7
        buffer_values = hash_initials;

        // message schedule
        for (int i = 0; i < 64; i++)
        {
            if (i < 16)
            {
                message_schedule = stoul(block.substr(i * 32, 32), 0, 2);
            }
            else
            {
                message_schedule = sigama1(schedule_list[i - 2]) + schedule_list[i - 7] + sigma0(schedule_list[i - 15]) + schedule_list[i - 16];
                message_schedule = message_schedule % (unsigned long)pow(2, 32);
            }
            schedule_list[i] = message_schedule;

            t1 = SIGMA1(buffer_values[4]) + choice(buffer_values[4], buffer_values[5], buffer_values[6]) + buffer_values[7] + constants[i] + message_schedule;
            t2 = SIGMA0(buffer_values[0]) + majority(buffer_values[0], buffer_values[1], buffer_values[2]);
            t1 = t1 % (unsigned long)pow(2, 32);
            t2 = t2 % (unsigned long)pow(2, 32);

            // rotate right 1 time and chnaging value of 'a'
            rotate(buffer_values.begin(), buffer_values.begin() + 7, buffer_values.end());
            buffer_values[0] = (t1 + t2) % (unsigned long)pow(2, 32);

            // changing value of 'e'
            buffer_values[4] = (buffer_values[4] + t1) % (unsigned long)pow(2, 32);
        }
        for (size_t j = 0; j < 8; j++)
        {
            hash_initials[j] = (hash_initials[j] + buffer_values[j]) % (unsigned long)pow(2, 32);
        }
        block_number++;
    }
    std::stringstream stream;
    for (int i = 0; i < 8; i++)
    {
        stream << std::setfill('0') << std::hex << hash_initials[i];
    }
    return stream.str();
}

// Functions for command line
int has(string s, int n, char **arr);
void fileWout(int n, char **arr);
void fileO(int n, char **arr);
void textWout(int n, char **arr);
void textO(int n, char **arr);

int main(int argc, char **argv)
{
    // if not help
    if (argc > 1 && has("-h", argc, argv) == -1 && has("--help", argc, argv) == -1)
    {
        // input is file
        if (has("-f", argc, argv) != -1)
        {
            // output is file
            if (has("-o", argc, argv) != -1)
            {
                fileWout(argc, argv);
            }
            // output is stdout
            else
            {
                fileO(argc, argv);
            }
        }
        // input is from stdin
        else
        {
            // output is file
            if (has("-o", argc, argv) != -1)
            {
                textWout(argc, argv);
            }
            // output is stdout
            else
            {
                textO(argc, argv);
            }
        }
    }
    // help
    else
    {
        string help = "\
./hash : Tool to find sha256 sum of text or file \n\
\n\
Syntax: \n\
./hash [-fo] [filename|text] \n\
\n\
Usage: \n\
./hash text \n\
./hash text -o outputFile \n\
./hash -f fileName \n\
./hash -f fileName -o outputFile \n\
";
        cout << help;
    }
    return 0;
}

// finding substring index
int has(string s, int n, char **arr)
{
    auto ind = find(arr, arr + n, s);
    return ind != arr + n ? distance(arr, ind) : -1;
}

// if input and output both are files
void fileWout(int n, char **arr)
{
    string fileName = arr[has("-f", n, arr) + 1];
    string outName = arr[has("-o", n, arr) + 1];

    ifstream file(fileName, fstream::binary);
    stringstream t;
    t << file.rdbuf();
    file.close();
    string o = sha256(t.str());

    ofstream out(outName, fstream::trunc | fstream::binary);
    out << o;
    out.close();
}

// if input is file and output is to be printed
void fileO(int n, char **arr)
{
    string fileName = arr[has("-f", n, arr) + 1];

    ifstream file(fileName, fstream::binary);
    stringstream t;
    t << file.rdbuf();
    file.close();
    string o = sha256(t.str());

    cout << o << endl;
}

// if input is from stdin and output is file
void textWout(int n, char **arr)
{
    string outName = arr[has("-o", n, arr) + 1];
    string t;
    if (arr[1] == "-o")
    {
        t = arr[3];
    }
    else
    {
        t = arr[1];
    }
    string o = sha256(t);

    ofstream out(outName, fstream::trunc | fstream::binary);
    out << o;
    out.close();
}

// if input is from stdin and output is to be printed
void textO(int n, char **arr)
{
    string t = arr[1];
    string o = sha256(t);
    cout << o << endl;
}