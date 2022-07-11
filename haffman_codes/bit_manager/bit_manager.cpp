#include <cmath>

#include <bitset>
#include <iostream>

#include "bit_manager.h"
using std::cout;
using std::endl;
using std::bitset;

bool BitManager::Read(byte &value, std::optional<size_t> n_bits, std::optional<size_t> index_start)
{
    if (n_bits && (*n_bits > 8 || *n_bits <= 0))
    {
        return false;
    }
    if (index_start && (*index_start > n_data_bits - 1 || *index_start < 0))
    {
        return false;
    }
    if (index_start)
    {
        cursor_read = *index_start;
    }
    if (!n_bits)
    {
        *n_bits = 8;
    }
    else if (cursor_read + *n_bits > n_data_bits)
    {
        return false;
    }
    value = 0;
    int rest = cursor_read % 8;
    int index = cursor_read / 8;
    // чтение из двух смежных элементов вектора
    if (8 - (cursor_read % 8) < *n_bits)
    {
        value |= data[index] & ((int)pow(2, 8 - rest) - 1);
        value <<= (*n_bits - (8 - rest));
        value |= (data[index + 1] >> (16 - *n_bits - rest));
    }
    // чтение из одного элемента вектора
    else
    {
        value |= (data[index] >> (8 - rest - *n_bits)) & ((int)pow(2, *n_bits) - 1);
    }
    cursor_read += *n_bits;
    return true;
}

bool BitManager::Write(byte value, std::optional<size_t> n_bits, std::optional<size_t> index_start)
{
    if (n_bits && (*n_bits > 8 || *n_bits <= 0))
    {
        return false;
    }
    if (index_start && (*index_start > n_data_bits || *index_start < 0))
    {
        return false;
    }
    if (index_start)
    {
        cursor_write = *index_start;
    }
    if (!n_bits)
    {
        *n_bits = 8;
    }
    if (cursor_write + *n_bits > n_data_bits)
    {
        data.push_back(0);
    }
    int rest = cursor_write % 8;
    int index = cursor_write / 8;
    // чтение в два смежных элементов вектора
    if (8 - cursor_write % 8 < *n_bits)
    {

        // зануляем часть значений в конце первой половины
        data[index] &= (((int)pow(2, rest) - 1) << (8 - rest)); 

        // заносим туда первую часть value (с учетом n_bits)
        data[index] |= ((value >> *n_bits - 8 + rest) & ((int)pow(2, 8 - rest) - 1)); 

        // зануляем часть значений в начале второй половины
        data[index + 1] &= ((int)pow(2, 16 - *n_bits - rest) - 1);

        // заносим туда вторую часть value (до конца)
        data[index + 1] |= (value << (16 - *n_bits - rest));
    }
    // чтение из одного элемента вектора
    else
    {
        // маска вида 11000111 для занесения вместо нулей value
        byte mask = 0;
        for (size_t i = 0; i < 8; ++i)
        {
            if (i >= rest && i < rest + *n_bits)
            {
                mask = mask << 1;
            }
            else
            {
                mask = (mask << 1) | 1;
            }
        } 
        // занесение в "окно из нулей" value
        data[index] = (data[index] & mask) | (value & ((int)pow(2, *n_bits) - 1)) << (8 - rest - *n_bits);
    }
    if (n_data_bits < cursor_write + *n_bits)
    {
        n_data_bits = cursor_write + *n_bits;
    }
    cursor_write += *n_bits;
    return true;
}
