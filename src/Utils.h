#pragma once

#include <initializer_list>
#include <random>
#include <list>


namespace utils
{

// Singleton to generate random integers and real numbers
class RandomGenerator
{
public:
    // Instance
    static RandomGenerator& Instance();

    // Generating integers from min to max
    int GetIntValue(int min, int max);

    // Generating real numbers from min to max
    float GetRealValue(int min, int max);
private:
    // Parameter for the distribution function
    std::mt19937 mGen;

    RandomGenerator();
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(RandomGenerator&) = delete;
};

//------------------------------------------------------------------------------------
// Class for loop iteration.
template<typename T>
class RecursiveList
{
    using List = std::list<T>;
    List mElementList;
    typename List::iterator mElementListIt = mElementList.begin();

public:
    RecursiveList() = default;
    RecursiveList(const std::list<T>& list)
    {
        InitList(list);
    }
    
    void InitList(const std::list<T>& list)
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

//------------------------------------------------------------------------------------
// Singleton for calculating cosines and sines of corners
class CosSinCalc
{
public:
    // Instance
    static CosSinCalc& Instance();

    // Method to get cos of corner
    float Cos(int angle);
    // Method to get sin of corner
    float Sin(int angle);
private:
    CosSinCalc();
    CosSinCalc(const CosSinCalc&) = delete;
    CosSinCalc& operator=(CosSinCalc&) = delete;

    // Map for storing cosines
    std::map<int, float> mCos;

    // Map for storing sines
    std::map<int, float> mSin;

    void CorrectAngle(int& angle);
};

//------------------------------------------------------------------------------------
struct Rect
{
    int mX;
    int mY;
    int mHeight;
    int mWidth;
    
    Rect();
    Rect(int x, int y, int height, int width);
};

//------------------------------------------------------------------------------------
// Method to draw texture
void DrawTexture(const std::string& texture);

// Method to get texture
Render::Texture* GetTexture(const std::string& name);

// Method for calculating the size of the object
int InitSize(Render::Texture* tex, float& delta_x, float& delta_y);

// Method for calculating the angle between vectors
float VectorsAngle(float x1, float x2, float x3, float x4);


}
