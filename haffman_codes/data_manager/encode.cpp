#include <iostream>
#include <queue>

#include "data_manager.h"

std::vector<byte> Encoder::GetProcessedData()
{
    if (data.size() == 0)
    {
        return {};
    }
    CreateTableFrequency();
    WritePowerAlphabet();
    // запись 0000 во 2-й байт (для WriteLastBitsData())
    bitDirector.Write(0, 4, 8); 


    BuildTree();
    CreateTableCodes();

    for (auto &[byte_, vec] : tableCodes)
    {
        std::cout << byte_ << " : ";
        for (byte bit : vec)
        {
            std::cout << static_cast<int>(bit) << " ";
        }
        std::cout << std::endl;
    }

    WriteCompressedData();
    WriteLastBitsData();

    return bitDirector.GetData();
}

void Encoder::CreateTableFrequency()
{
    for (byte byte_ : data)
    {
        ++tableFreq[byte_];
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

void Encoder::PostOrder(Node *node, std::vector<byte> &vecCodes)
{
    if (node->left)
    {
        vecCodes.push_back(0);
        PostOrder(node->left, vecCodes);
    }
    if (node->right)
    {
        vecCodes.push_back(1);
        PostOrder(node->right, vecCodes);
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
    PostOrder(root, vecCodes);
}

void Encoder::WritePowerAlphabet()
{
    bitDirector.Write(static_cast<byte>(tableFreq.size()), 8, 0);
}

void Encoder::WriteCompressedData()
{
    for (byte byte_ : data)
    {
        for (byte bit : tableCodes[byte_])
        {
            bitDirector.Write(bit, 1);
        }
    }
}

void Encoder::WriteLastBitsData()
{
    byte nSignificBits = bitDirector.GetLenBitSequence() % 8;
    bitDirector.Write(nSignificBits, 4, 8); // 8 - start_index
}