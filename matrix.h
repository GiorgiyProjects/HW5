#include <iostream>
#include <assert.h>
#include <map>
#include<vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
template<typename T, const T DefaultValue> class Matrix;
template<typename T, const T DefaultValue> class Element;

template<typename T, const T DefaultValue>
std::unordered_map<size_t,std::vector<Element<T, DefaultValue>>> MatrixElementsPool;

template<typename T, const T DefaultValue>
class Element
{
private:
    static const T mDefaultValue = DefaultValue;
    std::tuple<int, int, T> mRCV; // row column value
    bool mAmDefault = false;  // stores if it's fake default item or real deal
    Matrix<T, DefaultValue>* mFather; // stores pointer to father

public:
    Element() = default;
    ~Element() = default;
    
    void SetColumn(const size_t col)
    {
        get<1>(mRCV) = col;
    }

    void SetFather(Matrix<T, DefaultValue>* father)
    {
        mFather = father;
    }

    void SetDefault(const bool isdef)
    {
        mAmDefault = isdef;
    }

    void SetRow(const size_t row)
    {
        get<0>(mRCV) = row;
    }

    void SetVal(const T val)
    {
        get<2>(mRCV) = val;
    }

    bool isDef()
    {
        return mAmDefault;
    }

    size_t GetRow() const
    {
        return get<0>(mRCV);
    }

    size_t GetColumn() const
    {
        return get<1>(mRCV);
    }

    T GetVal() const
    {
        return get<2>(mRCV);
    }

    Element<T, DefaultValue>& operator=(const T& val) {
        if (!mAmDefault && (val != mDefaultValue)) //this element already exists, we are just changing value
        {
            SetVal(val);
            return (*this);
        } else
        // this element didnt exist, but now we need to add it
        if (mAmDefault && (val != mDefaultValue)) {
            Element<T, DefaultValue>* NewEl = mFather->CopyDefaultElementToVec(GetRow(), GetColumn(), val);
            auto a = MatrixElementsPool<T, DefaultValue>;
            std::sort(MatrixElementsPool<T, DefaultValue>[mFather->GetId()].begin(), MatrixElementsPool<T, DefaultValue>[mFather->GetId()].end(), std::less<Element<T,DefaultValue>>());
            a = MatrixElementsPool<T, DefaultValue>;
            return (*NewEl);
        } else
        // this element existed but now has to be defaulted
        if (!mAmDefault && (val == mDefaultValue))
        {
            mFather->erase(*this);
            mFather->mDefaultElement.SetRow(GetRow());
            mFather->mDefaultElement.SetColumn(GetColumn());

            return mFather->mDefaultElement;
        }
        return (*this);
    }
};

template<typename T, const T DefaultValue>
std::ostream& operator<< (std::ostream &out, const Element<T, DefaultValue> &el)
{
    out << el.GetVal();
    return out;
}

template<typename T, const T DefaultValue>
bool operator==(const Element<T, DefaultValue>& left, const int& right) {
    return left.GetVal() == right;
}

template<typename T, const T DefaultValue>
bool operator< (const Element<T, DefaultValue>& a, const Element<T, DefaultValue>& b) {
    if (a.GetRow() < b.GetRow()) return true;
    if (a.GetRow() > b.GetRow()) return false;
    return (a.GetColumn() < b.GetColumn());
}

template<typename T, const T DefaultValue>
class ElementLinkList
{
private:
    std::vector<Element<T, DefaultValue>*> mElementLinks;
    size_t mRow;
    Matrix<T, DefaultValue>* mFather; // stores pointer to father
    static const T mDefaultValue = DefaultValue;

public:
    ElementLinkList(int row, Matrix<T, DefaultValue>* Father)
    {
        mRow = row;
        mFather = Father;
    }
    ~ElementLinkList() = default;

    Element<T, DefaultValue>& operator[]( size_t col ) {
        //std::cout << "I am second bracket" << std::endl;
        // that means we are starting new row
        if ((mElementLinks.size() == 1) && (mElementLinks[0]->isDef()))
        {
            mElementLinks[0]->SetColumn(col);
            return (*mElementLinks[0]);
        }
        // we are able to find row and column
        for (auto& el:mElementLinks)
        {
            if (el->GetColumn() == col)
                return (*el);
        }
        // we found row, but didnt find column
        mFather->mDefaultElement.SetColumn(col);
        return mFather->mDefaultElement;
    }

    void append(Element<T, DefaultValue>* el)
    {
        mElementLinks.push_back(el);
    }

    bool empty()
    {
        return (mElementLinks.size() == 0);
    }

};

template<typename T, const T DefaultValue>
class Matrix
{
private:
    typedef typename std::vector<Element<T, DefaultValue>>::iterator iterator;
    T mDefaultValue = DefaultValue;
    size_t mId;
public:
    Element<T, DefaultValue> mDefaultElement; // that element stores default value

    size_t GetId()
    {
        return mId;
    }

    Matrix(size_t id) {
        mId = id;
        mDefaultElement.SetDefault(true);
        mDefaultElement.SetVal(mDefaultValue);
        mDefaultElement.SetFather(this);
    }
    ~Matrix() = default;

    size_t size() const
    {
        return MatrixElementsPool<T, DefaultValue>[mId].size();
    }

    Element<T, DefaultValue>* CopyDefaultElementToVec(int row, int col, T val)
    {
        Element<T, DefaultValue> NewElem;
        NewElem.SetDefault(false);
        NewElem.SetFather(this);
        NewElem.SetRow(row);
        NewElem.SetColumn(col);
        NewElem.SetVal(val);
        MatrixElementsPool<T, DefaultValue>[mId].push_back(NewElem);

        return &MatrixElementsPool<T, DefaultValue>[mId][MatrixElementsPool<T, DefaultValue>[mId].size() - 1];
    }

    void erase(Element<T, DefaultValue>& el)
    {
        Element<T, DefaultValue>* to_delete = &el;
        MatrixElementsPool<T, DefaultValue>[mId].erase(MatrixElementsPool<T, DefaultValue>[mId].begin() + (to_delete - &MatrixElementsPool<T, DefaultValue>[mId][0]));
        return;
    }

    ElementLinkList<T, DefaultValue> operator[]( size_t row ) {
        ElementLinkList<T, DefaultValue> PossibleElems(row, this);
        for (auto& el:MatrixElementsPool<T, DefaultValue>[mId])
        {
            Element<T, DefaultValue>* el_ptr = (&el);
            if (el.GetRow() == row)
                PossibleElems.append(el_ptr);
        }

        if (PossibleElems.empty()) {
            mDefaultElement.SetRow(row);
            Element<T, DefaultValue>* el_ptr = (&mDefaultElement);
            PossibleElems.append(el_ptr);
        }
        //std::cout << "I am first bracket" << std::endl;
        return PossibleElems;
    }


    // reload iterate over elements
    auto begin() const
    {
        return MatrixElementsPool<T, DefaultValue>[mId].begin();
    }

    auto end() const
    {
        return MatrixElementsPool<T, DefaultValue>[mId].end();
    }
};