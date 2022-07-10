#include <vector>
#include <optional>

using byte = unsigned char;

// int <-> size_t
// enum { CURSOR_SET = 1, CURSOR_END };
// constexpr int CURSOR_END = 2;
// constexpr int CURSOR_SET = 1;

class BitManager
{
public:
    BitManager() : cursor_read(0), cursor_write(0) {}
    explicit BitManager(std::vector<byte> &_data) : data(_data), cursor_read(0), cursor_write(data.size()), n_data_bits(data.size() * 8) {}

    bool Read(byte &value, std::optional<size_t> n_bits = std::nullopt, std::optional<size_t> index_start = std::nullopt);

    bool Write(byte value, std::optional<size_t> n_bits = std::nullopt, std::optional<size_t> index_start = std::nullopt);

    size_t SizeData() { return data.size(); }
private:
    std::vector<byte> data;
    size_t n_data_bits;
    size_t cursor_read;
    size_t cursor_write;
};