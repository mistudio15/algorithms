#include <iostream>
#include <bitset>

#include "stream.h"
#include "bit_manager.h"

void Encode(IInputStream &original, IOutputStream &compressed)
{
    // compressed.Write('s');
}

void Decode(IInputStream &compressed, IOutputStream &original)
{

}

int main()
{
    // InputStream original("hello mikhail you are amazing");
    // OutputStream compressed;
    // Encode(original, compressed);
    // std::vector<byte> vec = compressed.GetData();
    // std::cout << vec[0] << std::endl;
    
    std::vector<byte> vec = {98, 87, 234};
    for (byte val : vec)
    {
        std::cout << std::bitset<8>(val) << " | ";
    }
    std::cout << std::endl;
    BitManager bit(vec);
    byte v;
    std::cout << bit.Read(v, 8, 14) << std::endl;
    std::cout << std::bitset<8>(v) << std::endl;

    return 0;
}