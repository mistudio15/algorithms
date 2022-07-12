#include <iostream>

#include "data_manager.h"

void DataManager::postOrder(Node *node, void (*f) (Node *node))
{
    if (node)
    {
        postOrder(node->left, f);
        postOrder(node->right, f);
        f(node);
    }
}

DataManager::~DataManager()
{
    postOrder(root, [](Node *node){
        delete node;
    });
}

void DataManager::AddData(byte val)
{
    data.push_back(val);
}

void DataManager::AddData(std::vector<byte> const &vec)
{
    int size = data.size();
    std::copy(vec.begin(), vec.end(), data.begin() + size);
}