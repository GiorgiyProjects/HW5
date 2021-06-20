#include <iostream>
#include <assert.h>
#include <map>
#include<vector>

template<typename T, const T DefaultValue>
class Column
{
    typedef typename std::tuple<int,int,T> triplet;
    std::map<int, T> mVals;
    static const T mDefaultValue = DefaultValue;

public:
    Column() = default;
    ~Column() = default;

    void EraseDefaultKeys()
    {
        std::vector<int> DefaultValueKeys;
        for (const auto& v:mVals)
        {
            if (v.second == mDefaultValue)
            {
                DefaultValueKeys.push_back(v.first);
            }
        }
        for (const auto& val:DefaultValueKeys)
        {
            mVals.erase(val);
        }
    }

    int size()
    {
        EraseDefaultKeys();
        return mVals.size();
    }

    T& operator[] (int i)
    {
        if (mVals.find(i) == mVals.end())
            mVals[i] = mDefaultValue;
        return mVals[i];
    }

    void GetColumnValues(std::vector<triplet>& mAllValues, int row)
    {
        for (const auto& val:mVals)
            mAllValues.push_back(std::tie(row, val.first, val.second));
    }
};

template<typename T, const T DefaultValue>
class Matrix
{
    typedef typename std::tuple<int,int,T> triplet;
    typedef typename std::vector<triplet>::iterator iterator;

    std::map<int, Column<T,DefaultValue>> mRows;
    static const T mDefaultValue = DefaultValue;
    std::vector<triplet> mAllValues;

public:
    Matrix() = default;
    ~Matrix() = default;

    int size()
    {
        int size = 0;
        for (auto& row : mRows)
        {
            size += row.second.size();
        }
        return size;
    }

    // reload [][]
    Column<T, mDefaultValue>& operator[]( int i ) {
        if (mRows.find(i) == mRows.end())
        {
            mRows[i][0] = mDefaultValue;
        }
        return mRows[i];
    }

    void GetAllNonDefValues()
    {
        EraseDefaultKeys();
        mAllValues.clear();
        for (auto& row : mRows)
        {
            row.second.GetColumnValues(mAllValues, row.first);
        }
        return;
    }

    void EraseDefaultKeys()
    {
        for (auto val : mRows)
        {
            mRows[val.first].EraseDefaultKeys();
        }
    }

    // reload iterate over elements
    iterator begin()
    {
        GetAllNonDefValues();
        return mAllValues.begin();
    }

    iterator end()
    {
        return mAllValues.end();
    }
};