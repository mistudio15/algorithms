#pragma once

#include <vector>
#include <optional>

using byte = unsigned char;

class BitManager
{
public:
    BitManager() : cursor_read(0), cursor_write(0), n_data_bits(0) {}
    explicit BitManager(std::vector<byte> &_data) : data(_data), cursor_read(0), cursor_write(data.size() * 8), n_data_bits(data.size() * 8) {}

    bool Read(byte &value, std::optional<size_t> n_bits = std::nullopt, std::optional<size_t> index_start = std::nullopt);

    bool Write(byte value, std::optional<size_t> n_bits = std::nullopt, std::optional<size_t> index_start = std::nullopt);

    size_t GetLenBitSequence() { return n_data_bits; }
    std::vector<byte> GetData() { return data; }
    // bool setCursor(size_t index);
private:
    std::vector<byte> data;
    size_t n_data_bits;
    size_t cursor_read;
    size_t cursor_write;
};