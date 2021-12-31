#include <iostream>
#include <fstream>
#include <sstream>
#include "Cache.h"
using namespace std;

string hexToBinary(string address)
{
    string binAddress = "";
    int i;
    if (address[0] == '0' && address[1] == 'x')
        i = 2;
    else
        i = 0;
    for (i; i < address.length(); i++)
    {
        switch (address[i])
        {
        case '0':
            binAddress.append("0000");
            break;
        case '1':
            binAddress.append("0001");
            break;
        case '2': 
            binAddress.append("0010");
            break;
        case '3': 
            binAddress.append("0011");
            break;
        case '4':
            binAddress.append("0100");
            break;
        case '5':
            binAddress.append("0101");
            break;
        case '6':
            binAddress.append("0110");
            break;
        case '7':
            binAddress.append("0111");
            break;
        case '8':
            binAddress.append("1000");
            break;
        case '9':
            binAddress.append("1001");
            break;
        case 'a':
        case 'A':
            binAddress.append("1010");
            break;
        case 'b':
        case 'B':
            binAddress.append("1011");
            break;
        case 'c':
        case 'C':
            binAddress.append("1100");
            break;
        case 'd':
        case 'D':
            binAddress.append("1101");
            break;
        case 'e':
        case 'E':
            binAddress.append("1110");
            break;
        case 'f':
        case 'F':
            binAddress.append("1111");
            break;
        }
    }
    return binAddress;
}

void readFile(Cache& c, string fileName)
{
    ifstream file(fileName, ifstream::in);

    if (file.is_open())
    {
        string fileLine;
        string address;

        while (getline(file, fileLine))
        {
            string tempchar;
            string byte;
            istringstream stream(fileLine);

            getline(stream, tempchar, ' ');
            getline(stream, address, ' ');
            getline(stream, byte);

            string binAddress = hexToBinary(address);
            
            if (c.getNumSets() == 1)
                c.fullyAssociative(binAddress);
            else if (c.getNumBlocks() == 1)
                c.directMapped(binAddress);
            else
                c.setAssociative(binAddress);
        }
    }
    file.close();
}

int main()
{
    string fileName;
    int numSets, numBlocks, numBytes;
    int loop = 0;

    cout << "Enter trace file: ";
    cin >> fileName;
    while (loop < 16)
    {
        cout << "Enter # of sets in cache: ";
        cin >> numSets;
        cout << "Enter # of blocks per set: ";
        cin >> numBlocks;
        cout << "Enter # of bytes per block: ";
        cin >> numBytes;
        cout << endl;

        Cache c1("FIFO", numSets, numBlocks, numBytes);
        Cache c2("LRU", numSets, numBlocks, numBytes);
        readFile(c1, fileName);
        readFile(c2, fileName);

        if (c1.getNumSets() == 1)
        {
            c1.displayStats(fileName, "Fully Associative");
            c2.displayStats(fileName, "Fully Associative");
        }
        else if (c1.getNumBlocks() == 1)
            c1.displayStats(fileName, "Direct Mapped");
        else
        {
            c1.displayStats(fileName, "Set Associative");
            c2.displayStats(fileName, "Set Associative");
        }
        loop++;
    }
}