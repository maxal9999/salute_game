#pragma once

#include <initializer_list>
#include <list>


namespace utils
{

// Class for loop iteration.
template<typename T>
class RecursiveList
{
    using List = std::list<T>;
    List mElementList;
    typename List::iterator mElementListIt = mElementList.begin();

public:
    RecursiveList() = default;
    RecursiveList(std::initializer_list<T> list)
    {
		InitList(list);
    }
    
    void InitList(const std::initializer_list<T>& list)
    {
		for(auto& element : list)
			mElementList.emplace_back(element);
		mElementListIt = mElementList.begin();
    }
    
    void Add(const T& element)
    {
		mElementList.emplace_back(element);
    }
    
    void Next()
    {
		++mElementListIt;
		if (mElementListIt == mElementList.end())
			mElementListIt = mElementList.begin();
    }
    
    void Prev()
    {
		if (mElementListIt == mElementList.begin())
			mElementListIt = mElementList.end();
		--mElementListIt;
    }
    
    T Value(){ return *mElementListIt; }
    
    List& GetList(){ return mElementList; }
};

struct Rect
{
    int mX;
    int mY;
    int mHeight;
    int mWidth;
    
    Rect();
    Rect(int x, int y, int height, int width);
};


void DrawTexture(const std::string& texture);

}
