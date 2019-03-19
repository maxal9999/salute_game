#pragma once

/**
 * \file
 * \brief Helper classes
 * \author Maksimovskiy A.S.
 */

#include <random> 

// Singleton to generate random integers and real numbers
class RandomGenerator
{
public:
    // Instance
    static RandomGenerator& Instance();
    
    // Generating integers from min to max
    int GenIntValue(int min, int max);
    
    // Generating real numbers from min to max
    float GetRealValue(int min, int max);
private:
    // Parameter for the distribution function
    std::mt19937 mGen;
    
    RandomGenerator();
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(RandomGenerator&) = delete;
};

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

namespace object_params
{
     // Method for calculating the size of the object
     int InitSize(Render::Texture* tex, float& delta_x, float& delta_y);
     
     // Method for calculating the angle between vectors
     float VectorsAngle(float x1, float x2, float x3, float x4);
     
     Render::Texture* GetText(const std::string& name);
}