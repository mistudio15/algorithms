#include "stream.h"

// char <-> uchar
std::vector<byte> StringToVector(std::string &str)
{
    std::vector<byte> vec;
    for (byte sym : str)
    {   
        vec.push_back(sym);
    }
    return vec;
}

bool InputStream::Read(byte &value)
{
    if (cursor < data.size())
    {
        value = data[cursor];
        ++cursor;
        return true;
    }
    return false;
}

void OutputStream::Write(byte value)
{
    data.push_back(value);
    ++cursor;
}