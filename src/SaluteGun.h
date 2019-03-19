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

// Angle adjustment. Depends on the inclination of the cannon on the texture.
enum class AngleCorrect
{
    SMALL = 30,
    MIDDLE = 45,
    LARGE = 60
};

// Struct for create rocket by init params
struct RocketParams
{
    int mX;
    int mY;
    float mRotateAngle;
    float mDistance;
    int mLevel;

    RocketParams(int x, int y, float angle, float distance, int level);
};

//------------------------------------------------------------------------------------

// Base structure to describe the rocket
struct Rocket
{
    Rocket();
    Rocket(const RocketParams& params);
    
    // Calculation of the angle of rotation of the rocket and the initial coordinates
    void CalcAngles(float rotate_angle);

    // Create new rockets for continue salute
    std::list<RocketParams> CreateSubRockets(int level_limit);

	// Rocket drawing.
	// Accepts a link to the object vector object.
	void Draw();

	// Draw all effects
	void DrawEffects(EffectsContainer& eff_cont);
    
    // Method for simple drawing of a rocket
    void SimpleDraw();

	// Distance rocket fly
	float mDistance;

	// Flag denoting the moment of a rocket shot
	bool mFirstDraw;

	// Rocket fly effect
	ParticleEffectPtr mFlyEffect;

	// The flag is responsible for the paused in the rocket moving.
	bool mIsPaused;

	// The flag is responsible for the use of rocket.
	bool mIsUsed;

	// Rocket size
	utils::Rect mRect;
	utils::Rect mInitRect;

	// Rocket salute effect
	ParticleEffectPtr mSaluteEffect;

	// Tilt angle at which the rocket shot
	float mSystemAngle;
    
    // Rocket texture
	Render::Texture* mTexture;
    
protected:
    // Drag param
    float mCm;
    
    // Swift param
    float mKm;
private:
    // Level of the rockets
    int mLevel;

    // Flag the main rocket
    bool mMainRocket;

    // Array to store data about the previous position of the rocket
    float mXYold[N_DIM];
    
    // Function to calculate the coefficients by the method of Runge - Kutta
    float* RKFunc(float* xy_old);
    
    // Check that the rocket must to explode
    void CheckRocketOnUsed();

    // Rocket movement method
    void Move();
};

//------------------------------------------------------------------------------------
// Kind of rockets for salute
struct RedRocket : public Rocket
{
    RedRocket();
	RedRocket(const RocketParams& params);
private:
	void InitRocketParams();
};

//------------------------------------------------------------------------------------
// Weapon description class
class SaluteGun
{
public:
    SaluteGun();
    
    // Drawing weapons
    void Draw();

    // Initialization of rockets in the store
    void InitMinMaxPos(int min, int max);

    // Initialization of rockets in the store
    void InitRockets(bool restart = false);

    // Move salute gun
    void Move(bool is_left = true);

	void OnPausedMoving(bool pause = true);
    
    // Drawing all rockets fired
    void RocketsDraw(EffectsContainer& eff_cont, const std::string& limit_str);
    
    // Gun shot method
    bool Shot();
    
    // Gun size
    int Width() noexcept { return mRect.mWidth; }
    int Height() noexcept { return mRect.mHeight; }

private:
    using RocketPtr = std::unique_ptr<Rocket>;

    // Min and max X position for gun
    int mMinX;
    int mMaxX;

	// The flag is responsible for the paused in the rocket moving.
	bool mIsPaused;

    // Previous time to calculate rocket flight
    float mPrevTime;

    // Gun size and position
    utils::Rect mRect;

    // Rockets array.
    std::list<RocketPtr> mRocketPool;

    // Shot timer. The current gun shoots every 0.5 seconds.
    Core::Timer mShotTimer;

    // Gun texture
    Render::Texture* mTexture;

    // Timer for calculating the flight time of the rocket
    Core::Timer mWeaponTimer;

    // Width of the main window
    int mWinWidth;
};

}
