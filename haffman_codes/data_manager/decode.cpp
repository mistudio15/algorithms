#include <iostream>
#include <queue>

#include "data_manager.h"


void DataManager::AddData(byte val)
{
    data.push_back(val);
}

void DataManager::AddData(std::vector<byte> const &vec)
{
    int size = data.size();
    std::copy(vec.begin(), vec.end(), data.begin() + size);
}

std::vector<byte> Encoder::GetProcessedData()
{
    if (data.size() == 0)
    {
        return {};
    }
    CreateTableFrequency();

    //  отладка
    for (auto &[sym, freq] : tableFreq)
    {
        std::cout << "(" << sym << ", " << freq << "), ";
    }
    std::cout << std::endl;
    // конец

    WritePowerAlphabet();
    // запись 0000 во 2-й байт (для WriteLastBitsData())
    bitDirector.Write(0, 4, 8); 


    BuildTree();
    CreateTableCodes();

    for (auto &[sym, vec] : tableCodes)
    {
        std::cout << sym << " : ";
        for (byte val : vec)
        {
            std::cout << static_cast<int>(val);
        }
        std::cout << std::endl;
    }

    WriteCompressedData();
    WriteLastBitsData();

    return bitDirector.GetData();
}

void Encoder::CreateTableFrequency()
{
    for (byte sym : data)
    {
        ++tableFreq[sym];
    }
}


void Encoder::BuildTree()
{
    std::priority_queue<std::pair<size_t, Node *>, std::vector<std::pair<size_t, Node *>>, std::greater<std::pair<size_t, Node *>>> queueNodes; 
    for (auto &[sym, freq] : tableFreq)
    {
        Node *temp = new Node(sym, freq);
        queueNodes.push(std::make_pair(temp->freq, temp));
    }
    while (queueNodes.size() >= 2)
    {
        auto leftPair = queueNodes.top();
        queueNodes.pop();

        auto rightPair = queueNodes.top();
        queueNodes.pop();

        Node *mergeNode = new Node(leftPair.second, rightPair.second);
        queueNodes.push(std::make_pair(mergeNode->freq, mergeNode));
    }

    root = queueNodes.top().second;
}

void Encoder::PreOrder(Node *node, std::vector<byte> &vecCodes)
{
    if (node->left)
    {
        vecCodes.push_back(0);
        PreOrder(node->left, vecCodes);
    }
    if (node->right)
    {
        vecCodes.push_back(1);
        PreOrder(node->right, vecCodes);
    }
    if (!node->left && !node->right)
    {
        tableCodes[node->value] = vecCodes;
        vecCodes.pop_back();
        bitDirector.Write(1, 1);
        bitDirector.Write(node->value, 8);
    }
    else
    {
        vecCodes.pop_back();
        bitDirector.Write(0, 1);
    }
}

void Encoder::CreateTableCodes()
{
    std::vector<byte> vecCodes;
    PreOrder(root, vecCodes);
}

void Encoder::WritePowerAlphabet()
{
    bitDirector.Write(static_cast<byte>(tableFreq.size()), 8, 0);
}

void Encoder::WriteCompressedData()
{
    for (byte sym : data)
    {
        std::cout << "sym {" << sym << "} = ";
        for (byte bit : tableCodes[sym])
        {
            std::cout << static_cast<int>(bit) << " ";
            bitDirector.Write(bit, 1, bitDirector.GetLenBitSequence());
        }
        std::cout << std::endl;
    }
}

void Encoder::WriteLastBitsData()
{
    byte nSignificBits = bitDirector.GetLenBitSequence() % 8;
    bitDirector.Write(nSignificBits, 4, 8);
}