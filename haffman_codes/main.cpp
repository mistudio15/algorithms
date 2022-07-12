#include <iostream>
#include <bitset>
#include <string>
#include <fstream>

#include "stream.h"
#include "bit_manager.h"
#include "data_manager.h"

#define ERROR -1

void Encode(IInputStream &original, IOutputStream &compressed)
{
    Encoder encoder;
    byte value;
    while (original.Read(value))
    {
        encoder.AddData(value);
    }
    std::vector<byte> vecCompressed = encoder.GetProcessedData();
    if (!vecCompressed.size())
    {
        return;
    }
    for (byte byte_ : vecCompressed)
    {
        compressed.Write(byte_);
    }
}

void Decode(IInputStream &compressed, IOutputStream &original)
{
    Decoder decoder;
    byte value;
    while (compressed.Read(value))
    {
        decoder.AddData(value);
    }
    std::vector<byte> vecOriginal = decoder.GetProcessedData();
    if (!vecOriginal.size())
    {
        return;
    }
    for (byte byte_ : vecOriginal)
    {
        original.Write(byte_);
    }
}

int main(int argc, char *argv[])
{
    std::ifstream textFile("../" + std::string(argv[1]));
    if (!textFile)
    {
        return ERROR;
    }
    std::vector<byte> vecOriginal;
    char sym;
    while (textFile.get(sym))
    {
        vecOriginal.push_back(static_cast<byte>(sym));
    }
    textFile.close();

    InputStream original(vecOriginal);
    std::cout << "Original size = " << vecOriginal.size() << " byte" << std::endl;

    OutputStream compressed;
    // сжатие
    Encode(original, compressed);

    std::vector<byte> vecCompressed = compressed.GetData();
    std::cout << "Compressed size = " << vecCompressed.size() << " byte" << std::endl;
    std::cout << "Compressio ratio = " << static_cast<float>(vecOriginal.size()) / vecCompressed.size() << " byte" << std::endl;


    InputStream compressedCopy(compressed.GetData());
    OutputStream originalRestored;
    // распаковка
    Decode(compressedCopy, originalRestored);
    std::vector<byte> vecRestored = originalRestored.GetData();

    std::cout << "Matched : " << (vecOriginal == vecRestored ? "Yes" : "No") << std::endl; 
    return 0;
}