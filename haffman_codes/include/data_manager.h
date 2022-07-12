#pragma once

#include <vector>
#include <map>
#include <utility>

#include "bit_manager.h"

using byte = unsigned char;

struct Node
{
    Node() : left(nullptr), right(nullptr) {}
    Node(Node *left_, Node *right_) : left(left_), right(right_), freq(left_->freq + right_->freq) {}
    Node(byte value_) : value(value_), left(nullptr), right(nullptr) {}
    Node(byte value_, int freq_) : value(value_), freq(freq_), left(nullptr), right(nullptr) {}
    byte value;
    int freq;
    Node *left;
    Node *right;
};

inline bool operator>(std::pair<size_t, Node *> const &left, std::pair<size_t, Node *> const &right)
{
    return left.first > right.first;
}

class DataManager
{
public:
    virtual std::vector<byte> GetProcessedData() = 0;

    ~DataManager();

    void AddData(byte val);
    void AddData(std::vector<byte> const &vec);
protected:
    Node *root = nullptr;
    std::vector<byte> data;
    
    virtual void BuildTree() = 0;
private:
    void postOrder(Node *node, void (*f) (Node *node));
};

class Encoder : public DataManager
{
public:
    std::vector<byte> GetProcessedData() override;
private:
    BitManager                        bitDirector;
    std::map<byte, size_t>            tableFreq;
    std::map<byte, std::vector<byte>> tableCodes;

    void CreateTableFrequency();
    void BuildTree() override;


    // составление таблицы кодов и запись сжатого дерева
    void CreateTableCodes();
    void PostOrder(Node *node, std::vector<byte> &vecCodes);

    void WritePowerAlphabet();
    void WriteCompressedData();

    // 4 бита 2-го байта - кол-во значащих бит в последнем байте данных
    void WriteLastBitsData();
};

class Decoder : public DataManager
{
public:
    std::vector<byte> GetProcessedData() override;
private:
    BitManager         bitDirector;
    std::vector<byte>  decodedData;
    byte               powerAlphabet;
    size_t             nBitsData;

    void BuildTree() override;
    void DecodeData();
};