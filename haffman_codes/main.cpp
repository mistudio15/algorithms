#include <iostream>
#include <bitset>
#include <string>

#include "stream.h"
#include "bit_manager.h"
#include "data_manager.h"

void Encode(IInputStream &original, IOutputStream &compressed)
{
    Encoder encoder;
    byte value;
    while (original.Read(value))
    {
        std::cout << std::bitset<8>(value) << " | ";
        encoder.AddData(value);
    }
    std::cout << " end" << std::endl;
    std::vector<byte> vecRes = encoder.GetProcessedData();
    for (byte val : vecRes)
    {
        compressed.Write(val);
    }
}

void Decode(IInputStream &compressed, IOutputStream &original)
{

}

int main()
{
    std::string str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    InputStream original(str);
    std::cout << "LEN1 = " << str.size() << std::endl;
    OutputStream compressed;
    Encode(original, compressed);
    std::vector<byte> vec = compressed.GetData();
    std::cout << "LEN2 = " << vec.size() << std::endl;
    for (byte val : vec)
    {
        std::cout << std::bitset<8>(val) << " | ";
    }
    std::cout << std::endl;
    return 0;
}