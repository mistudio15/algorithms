#pragma once

#include <vector>
#include <map>

#include "bit_manager.h"

using byte = unsigned char;

struct Node
{
    Node() : left(nullptr), right(nullptr) {}
    Node(Node *left_, Node *right_) : left(left_), right(right_), freq(left_->freq + right_->freq) {}
    Node(byte value_, int freq_) : value(value_), freq(freq_), left(nullptr), right(nullptr) {}
    byte value;
    int freq;
    Node *left;
    Node *right;
};

inline bool operator>(const Node &left, const Node &right)
{
    return left.freq > right.freq;
} 

class DataManager
{
public:
    virtual std::vector<byte> GetProcessedData() = 0;

    void AddData(byte val);
    void AddData(std::vector<byte> const &vec);
protected:
    std::vector<byte> data;
    Node *root = nullptr;
};

class Encoder : public DataManager
{
public:
    std::vector<byte> GetProcessedData() override;
private:
    BitManager bitDirector;
    std::map<byte, size_t> tableFreq;
    std::map<byte, std::vector<byte>> tableCodes;

    void CreateTableFrequency();
    void BuildTree();
    void CreateTableCodes();

    // составление таблицы кодов и запись сжатого дерева
    void PreOrder(Node *node, std::vector<byte> &vecCodes);

    void WritePowerAlphabet();
    void WriteCompressedData();

    // 4 бита 2-го байта - кол-во значащих бит в последнем байте данных
    void WriteLastBitsData();
};