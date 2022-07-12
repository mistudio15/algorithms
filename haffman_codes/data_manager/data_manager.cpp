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