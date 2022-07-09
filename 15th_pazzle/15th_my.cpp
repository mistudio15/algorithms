#include <iostream>
#include <array>
#include <cassert>
#include <iterator>
#include <set>
#include <string>
#include <cstring>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>

const int FieldSize = 16;
const int RowSize = 4;
using array_b = std::array<char, FieldSize>;
using array_hb = std::array<char, FieldSize / 2>;

const array_b FinishField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class Field
{
public:
    explicit Field(array_b const &);
    
    bool operator==(Field const &) const;
    bool operator!=(Field const &) const;
    bool operator==(array_b const &arr) const;
    bool operator!=(array_b const &arr) const;
    char operator[](int) const;
    char const &rowAt(int) const;
    void Swap(int, int);
    size_t size() const { return FieldSize; }
    char Get(int) const;
    void Insert(char, int);
private:
    array_hb field8;
};

char const &Field::rowAt(int i) const
{
    return field8[i];
}

Field::Field(array_b const &array) : field8({0})
{
    for (size_t i = 0; i < array.size(); i += 2)
    {
        field8[i / 2] = (array[i] << 4) + array[i + 1];
    }
}

void showBit(char val)
{
    for (size_t i = 0; i < 8; ++i)
    {
        std::cout << ((val & 0x80) ? 1 : 0) << " ";
        val = val << 1;
    }
    std::cout << std::endl;
}


char Field::Get(int i) const
{
    if (i % 2 == 0)
    {
        return (field8[i / 2] >> 4) & 0x0F;
    }
    else
    {
        return field8[i / 2] & 0x0F;
    }   
}

void Field::Insert(char val, int i)
{
    if (i % 2 == 0)
    {
        field8[i / 2] = (field8[i / 2] & 0x0F) + (val << 4);
    }
    else
    {
        field8[i / 2] = (field8[i / 2] & 0xF0) + val;
    }
}

void Field::Swap(int iLeft, int iRight)
{
    char vLeft = Get(iLeft);
    char vRight = Get(iRight);
    Insert(vLeft, iRight);
    Insert(vRight, iLeft);
} 

bool Field::operator==(Field const &other) const
{
    return field8 == other.field8;
}

bool Field::operator!=(Field const &other) const
{
    return !(*this == other);
}

bool Field::operator==(array_b const &arr) const
{
    Field temp(arr);
    return (*this == temp);
}

bool Field::operator!=(array_b const &arr) const
{
    return (*this == arr);
}

char Field::operator[](int i) const
{
    return Get(i);
}

class Hasher;

class GameState
{
public:
    explicit GameState(array_b const &);

    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    bool operator==(const GameState &other) const;
    bool operator!=(const GameState &other) const;

    bool HasSolution() const;

    void Swap(int, int);
    bool IsFinish() const;
    char Heuristic() const;
    char ManhattanDistance(char, size_t) const; 

    friend std::ostream &operator <<(std::ostream &out, GameState &gState);
    friend Hasher;
private:
    Field field;
    char posZero;
};

GameState::GameState(const array_b &_field) :
    field(_field),
    posZero(-1)
{
    std::set<int> digits(FinishField.begin(), FinishField.end());
    for (size_t i = 0; i < field.size(); ++i)
    {
        auto it = digits.find(field[i]);
        if (it != digits.end())
        {
            digits.erase(it);
        }
    }  
    assert(digits.empty());
    
    for (size_t i = 0; i < field.size(); ++i)
    {
        if (field[i] == 0)
        {
            posZero = i;
            break;
        }
    }
    // posZero = static_cast<int>(std::distance(field.begin(), std::find(field.begin(), field.end(), 0)));
}

bool GameState::operator!=(const GameState &other) const
{
    return *this == other; 
}

bool GameState::operator==(const GameState &other) const
{
    return field == other.field;
} 

bool GameState::IsFinish() const
{
    return field == FinishField;
}

bool GameState::CanMoveLeft() const
{
    return posZero % RowSize != 0;
}

bool GameState::CanMoveUp() const
{
    return posZero >= RowSize;
}

bool GameState::CanMoveRight() const
{
    return posZero % RowSize != RowSize - 1;
}

bool GameState::CanMoveDown() const
{
    return posZero < FieldSize - RowSize;
}

bool GameState::HasSolution() const
{
    int count = 0;
    // std::vector<char> vec(field.begin(), field.end());
    std::vector<char> vec;
    for (size_t i = 0; i < field.size(); ++i)
    {
        vec.push_back(field[i]);
    }
    vec.erase(std::find(vec.begin(), vec.end(), 0));
    for (size_t i = 0; i < vec.size(); ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (vec[j] > vec[i])
            {
                ++count; 
            }
        }
    }
    if (FieldSize % 2 == 0)
    {
        count += (posZero / 4 + 1);
    }
    return !(count % 2);
}

void GameState::Swap(int l, int r) 
{
    assert(l >= 0 && l < FieldSize && r >= 0 && r < FieldSize);
    field.Swap(l, r);
}

GameState GameState::MoveLeft() const
{
    assert(CanMoveLeft());
    GameState tempState(*this);
    tempState.Swap(tempState.posZero, tempState.posZero - 1); 
    --tempState.posZero;
    return tempState;
}

GameState GameState::MoveUp() const
{
    assert(CanMoveUp());
    GameState tempState(*this);
    tempState.Swap(tempState.posZero, tempState.posZero - RowSize); 
    tempState.posZero -= RowSize;
    return tempState;
}

GameState GameState::MoveRight() const
{
    assert(CanMoveRight());
    GameState tempState(*this);
    tempState.Swap(tempState.posZero, tempState.posZero + 1); 
    ++tempState.posZero;
    return tempState;
}

GameState GameState::MoveDown() const
{
    assert(CanMoveDown());
    GameState tempState(*this);
    tempState.Swap(tempState.posZero, tempState.posZero + RowSize); 
    tempState.posZero += RowSize;
    return tempState;
}

char GameState::ManhattanDistance(char val, size_t pos) const
{
    --val;
    if (val == -1)
    {
        return abs(RowSize - 1 - pos / RowSize) + abs(RowSize - 1 - pos % RowSize);
    }
    return abs(val / RowSize - pos / RowSize) + abs(val % RowSize - pos % RowSize);
}

char GameState::Heuristic() const
{
    char sum = 0;
    for (size_t i = 0; i < field.size(); ++i)
    {
        sum += ManhattanDistance(field[i], i);
    }
    return sum;
}

class Hasher
{
public:
    std::size_t operator()(GameState const &gState) const
    {
        std::size_t hash = 0;
        memcpy(&hash, &gState.field.rowAt(0), sizeof(hash));
        return hash;
    }
};

class Less
{
public:
    bool operator() (std::pair<char, GameState> const &l, std::pair<char, GameState> const &r) const
    {
        return l.first < r.first;
    }
};

std::string GetResult(GameState const &startState, std::unordered_map<GameState, char, Hasher> const &visited);

std::string Competition(GameState const &startState)
{
    std::unordered_map<GameState, char, Hasher> visited;
    visited[startState] = 'S';
    std::set<std::pair<char, GameState>, Less> set;
    set.insert(std::make_pair(startState.Heuristic(), startState));
    if (!startState.HasSolution())
    {
        return std::string();
    }
    while (!set.empty())
    {
        if (set.size() > 20)
        {
            set.erase(--set.end());
        }

        GameState tempState = (*set.begin()).second;
        set.erase(set.begin());
        if (tempState.IsFinish())
        {
            break;
        }
        if (tempState.CanMoveLeft())
        {
            GameState newState = tempState.MoveLeft();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'L';
                set.insert(std::make_pair(newState.Heuristic(), newState));
            }
        }
        if (tempState.CanMoveUp())
        {
            GameState newState = tempState.MoveUp();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'U';
                set.insert(std::make_pair(newState.Heuristic(), newState));
            }
        }
        if (tempState.CanMoveRight())
        {
            GameState newState = tempState.MoveRight();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'R';
                set.insert(std::make_pair(newState.Heuristic(), newState));
            }
        }
        if (tempState.CanMoveDown())
        {
            GameState newState = tempState.MoveDown();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'D';
                set.insert(std::make_pair(newState.Heuristic(), newState));
            }
        }
    }
    return GetResult(startState, visited);
}

std::string GetResult(GameState const &startState, std::unordered_map<GameState, char, Hasher> const &visited)
{
    GameState tempState(FinishField);
    char move = visited.at(tempState);
    std::string result;
    while (move != 'S')
    {
        result += move;
        switch(move)
        {
            case 'L':
            {
                tempState = tempState.MoveRight();
                break;
            }
            case 'R':
            {
                tempState = tempState.MoveLeft();
                break;
            }
            case 'U':
            {
                tempState = tempState.MoveDown();
                break;
            }
            case 'D':
            {
                tempState = tempState.MoveUp();
                break;
            }
            default:
            {
                assert(false);
            }
        }
        move = visited.at(tempState);
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::ostream &operator <<(std::ostream &out, GameState &gState)
{
    for (size_t i = 0; i < RowSize; ++i)
    {
        for (size_t j = 0; j < RowSize; ++j)
        {
            out << std::setw(3) << static_cast<int>(gState.field[i * RowSize + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

int main()
{
    // GameState state({3, 8, 0, 5, 2, 1, 4, 6, 7});
    // GameState state({1, 2, 3, 4, 6, 5, 8, 7, 0});

    // GameState state({1, 2, 4, 3, 5, 6, 8, 7, 9, 10, 11, 12, 13, 14, 0, 15});
    // GameState state({
    //     1, 7, 9, 15,
    //     5, 6, 12, 10,
    //     4, 13, 14, 8,
    //     11, 2, 3, 0
    // });

    
    GameState state({
        6, 9, 3, 15,
        14, 4, 2, 8,
        10, 1, 5, 13,
        0, 11, 7, 12
    });

    std::string result = Competition(state);
    std::cout << state << std::endl;
    for (char move : result)
    {
        switch(move)
        {
            case 'U':
            {
                state = state.MoveUp();
                break;
            }
            case 'L':
            {
                state = state.MoveLeft();
                break;
            }
            case 'R':
            {
                state = state.MoveRight();
                break;
            }
            case 'D':
            {
                state = state.MoveDown();
                break;
            }
        }
        std::cout << state << std::endl;
    }
    std::cout << result << std::endl;
}