#include <iostream>
#include <stack>
#include <stack>

#include "data_manager.h"

std::vector<byte> Decoder::GetProcessedData()
{
    if (data.size() == 0)
    {
        return {};
    }
    for (byte byte_ : data)
    {
        bitDirector.Write(byte_);
    }
    bitDirector.Read(powerAlphabet);
    byte nSignificBits;
    bitDirector.Read(nSignificBits, 4);
    BuildTree();
    nBitsData = bitDirector.GetLenBitSequence() - bitDirector.GetCursorRead() - (8 - nSignificBits);
    DecodeData();
    return decodedData;
}

void Decoder::BuildTree()
{
    size_t cPowerAlph = powerAlphabet;
    std::stack<Node *> stackNodes;
    byte move;
    byte sym;
    while (cPowerAlph > 0 || stackNodes.size() != 1)
    {
        bitDirector.Read(move, 1);
        if (move == 1)
        {
            bitDirector.Read(sym, 8);
            stackNodes.push(new Node(sym));
            --cPowerAlph;
        }
        else if (move == 0)
        {
            Node *rightNode = stackNodes.top();
            stackNodes.pop();
            Node *leftNode = stackNodes.top();
            stackNodes.pop();
            Node *mergeNode = new Node(leftNode, rightNode);
            stackNodes.push(mergeNode); 
        }
    }
    root = stackNodes.top();
}

void Decoder::DecodeData()
{
    byte code;
    Node *tempNode = root;
    for (size_t i = 0; i < nBitsData; ++i)
    {
        bitDirector.Read(code, 1);
        tempNode = (code == 1 ? tempNode->right : tempNode->left);
        if (!tempNode->left && !tempNode->right)
        {
            decodedData.push_back(tempNode->value);
            tempNode = root;
        }
    }
}