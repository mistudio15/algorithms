#pragma once

#include <vector>
#include <string>
#include <cstdio>

using byte = unsigned char;

std::vector<byte> StringToVector(std::string &str);

class IInputStream
{
public:
    virtual bool Read(byte &value) = 0;
};

class IOutputStream
{
public:
    virtual void Write(byte) = 0;
};

class InputStream : public IInputStream
{
public:
    explicit InputStream(std::vector<byte> _data) : data(_data), cursor(0) {}
    explicit InputStream(std::string _data) : data(StringToVector(_data)), cursor(0) {}
    bool Read(byte &value) override;
private:
    std::vector<byte> data;
    size_t cursor;
};

class OutputStream : public IOutputStream
{
public:
    OutputStream() : cursor(0) {}
    void Write(byte value) override;
    std::vector<byte> GetData() const { return data; }
private:
    std::vector<byte> data;
    size_t cursor;
};
