#include <iostream>
#include "stream.h"

void Encode(IInputStream &original, IOutputStream &compressed)
{
    compressed.Write('s');
}

void Decode(IInputStream &compressed, IOutputStream &original)
{

}

int main()
{
    InputStream original("hello mikhail you are amazing");
    OutputStream compressed;
    Encode(original, compressed);
    std::vector<byte> vec = compressed.GetData();
    std::cout << vec[0] << std::endl;
    return 0;
}