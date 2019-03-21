#pragma once

/**
 * \file
 * \brief Classes to describe the behavior of guns and rockets
 * \author Maksimovskiy A.S.
 */

#include <future>
#include <list>
#include <memory>

#include "Params.h"
#include "Utils.h"


namespace weapons
{

// Struct for create rocket by init params
struct RocketParams
{
    // Position of the rocket
    int mX;
    int mY;
    // Rotate angle
    float mRotateAngle;
    // Level in a reaction chain
    int mLevel;
    // Name of the salute effect
    std::string mSaluteEffectName;
    // Main rocket flag
    bool mMainRocket;

    RocketParams(int x, int y, float angle, int level, 
                 const std::string& effect_name,
                 bool is_main = false);
};

//------------------------------------------------------------------------------------

// Base structure to describe the rocket
struct Rocket
{
    Rocket(const RocketParams& params);
    virtual ~Rocket() = default;
    
    // Calculation of the angle of rotation of the rocket and the initial coordinates
    void CalcAngles(float rotate_angle);

    // Ending all effects, delete pointer to texture
    bool Clear();

    // Create new rockets for continue salute
    std::list<RocketParams> CreateSubRockets(const std::string& salute_type, int level_limit);

    // Rocket drawing.
    // Accepts a link to the object vector object.
    void Draw();

    // Draw all effects
    void DrawEffects(EffectsContainer& eff_cont);
    
    // Method for simple drawing of a rocket
    void SimpleDraw();

    // Flag denoting the moment of a rocket shot
    bool mFirstDraw;

    // Rocket fly effect
    ParticleEffectPtr mFlyEffect;

    // The flag is responsible for the paused in the rocket moving.
    bool mIsPaused;

    // Flag the main rocket
    bool mMainRocket;
    
protected:
    // Drag param
    float mCm;

    // Corrective params
    float mDeltaX;
    float mDeltaY;
    
    // Swift param
    float mKm;

    // Rocket texture
    Render::Texture* mTexture;

private:
    // Array to store data about the previous position of the rocket
    float mXYold[N_DIM];
    
    // Function to calculate the coefficients by the method of Runge - Kutta
    float* RKFunc(float* xy_old);
    
    // Check that the rocket must to explode
    void CheckRocketOnUsed();

    // Rocket movement method
    void Move();

    // Distance rocket fly
    float mDistance;

    // The flag is responsible for the use of rocket.
    bool mIsUsed;

    // Level of the rockets
    int mLevel;

    // Rocket size
    utils::Rect mRect;
    utils::Rect mInitRect;

    // Rocket salute effect
    ParticleEffectPtr mSaluteEffect;
    std::string mSaluteEffectName;
};

//------------------------------------------------------------------------------------
// Kind of rockets for salute
struct RedRocket : public Rocket
{
    RedRocket(const RocketParams& params);
    virtual ~RedRocket() = default;

private:
    void InitRocketParams();
};

//------------------------------------------------------------------------------------
// Weapon description class
class SaluteGun
{
public:
    SaluteGun();
    ~SaluteGun() = default;
    
    // Drawing weapons
    void Draw();

    // Initialization of rockets in the store
    void InitMinMaxPos(int min, int max);

    // Initialization of rockets in the store
    void InitRockets(bool restart = false);

    // Move salute gun
    void Move(bool is_left = true);

    // Salute shot when mouse click
    bool MouseShot(int x, int y);

    // Change the flag on paused
    void OnPausedMoving(bool pause = true);
    
    // Drawing all rockets fired
    void RocketsDraw(EffectsContainer& eff_cont, const std::string& limit_str);

    // Set an effect of the rockets
    void SetEffect(const std::string& effect_name);
    
    // Gun shot method
    bool Shot(bool forced = false);

private:
    using RocketPtr = std::unique_ptr<Rocket>;

    // Timer for delay shot by mouse and space click
    Core::Timer mHandShotTimer;

    // The flag is responsible for the paused in the rocket moving.
    bool mIsPaused;

    // Min and max X position for gun
    int mMinX;
    int mMaxX;

    // Previous time to calculate rocket flight
    float mPrevTime;

    // Gun size and position
    utils::Rect mRect;

    // Rockets array.
    std::list<RocketPtr> mRocketPool;

    // Rocket salute effect name
    std::string mSaluteEffectName;

    // Shot timer
    Core::Timer mShotTimer;

    // Gun texture
    Render::Texture* mTexture;

    // Timer for calculating the flight time of the rocket
    Core::Timer mWeaponTimer;

    // Width of the main window
    int mWinWidth;
};

}
