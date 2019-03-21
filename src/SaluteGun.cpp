/**
 * \file
 * \brief Implementing classes to describe the behavior of weapons and rockets
 * \author Maksimovskiy A.S.
 */

#include "stdafx.h"

#include "SaluteGun.h"

#include <corecrt_math_defines.h>
#include <mutex>

#include "Utils.h"


namespace weapons
{

 // Time discrete
static float TIME_DELTA = 0.1f;

RocketParams::RocketParams(int x, int y, float angle, int level,
                           const std::string& effect_name,
                           bool is_main)
    : mX(x), mY(y), mRotateAngle(angle), mLevel(level), 
    mSaluteEffectName(effect_name), mMainRocket(is_main)
{
}

//------------------------------------------------------------------------------------
// Rocket

Rocket::Rocket(const RocketParams& params)
   : mFirstDraw(false),
    mIsPaused(false),
    mIsUsed(false),
    mMainRocket(params.mMainRocket),
    mDeltaX(0.0f),
    mDeltaY(0.0f),
    mLevel(params.mLevel),
    mSaluteEffectName(params.mSaluteEffectName)
{
    auto& inst = utils::RandomGenerator::Instance();
    if (mMainRocket)
        mDistance = inst.GetRealValue(MAIN_MIN_DISTANCE, MAIN_MAX_DISTANCE);
    else
        mDistance = inst.GetRealValue(MIN_DISTANCE, MAX_DISTANCE);
    mRect.mX = params.mX;
    mRect.mY = params.mY;
    mInitRect.mX = params.mX;
    mInitRect.mY = params.mY;
    CalcAngles(params.mRotateAngle);

    // Init salute name if mix type
    if (mSaluteEffectName == SALUTE_TYPE_FORTH)
    {
        int type_id = inst.GetIntValue(1, Config::SaluteCount());
        mSaluteEffectName = SALUTE_EFFECT + std::to_string(type_id);
    }
}

float* Rocket::RKFunc(float* xy_old)
{
    float* y = new float[N_DIM];

    y[0] = xy_old[1];
    // Vx change
    y[1] = -mCm * xy_old[1] - mKm * xy_old[3];
    y[2] = xy_old[3];
    // Vy change
    y[3] = -G - mCm * xy_old[3] + mKm * xy_old[1];

    return (y);
}

void Rocket::CalcAngles(float rotate_angle)
{
    float ang = rotate_angle * M_PI / PI_DEGREES;

    // Initial position, initial speed, shot angle
    int v = !mLevel ? ROCKET_VELOCITY : ROCKET_VELOCITY / 2;
    // x0
    mXYold[0] = mRect.mX;
    // vx0
    mXYold[1] = v * cos(ang);
    // y0
    mXYold[2] = mRect.mY;
    // vy0
    mXYold[3] = v * sin(ang);
}

bool Rocket::Clear()
{
    if (!mIsUsed)
        return false;

    mFlyEffect = nullptr;
    mSaluteEffect = nullptr;
    mTexture = nullptr;
    return true;
}

void Rocket::CheckRocketOnUsed()
{
    int curr_x = mRect.mX;
    int init_x = mInitRect.mX;
    int curr_y = mRect.mY;
    int init_y = mInitRect.mY;
    float distance = math::sqrt((curr_x - init_x) * (curr_x - init_x) +
                                (curr_y - init_y) * (curr_y - init_y));
    mIsUsed = distance >= mDistance;
}

std::list<RocketParams> Rocket::CreateSubRockets(const std::string& salute_type, int level_limit)
{
    if (!mIsUsed)
        return {};

    int new_level = ++mLevel;
    if (new_level > level_limit)
        return {};

    auto angle = acos(mXYold[3] / sqrt(mXYold[3] * mXYold[3] + mXYold[1] * mXYold[1]));
    float real_angle = angle * PI_DEGREES / M_PI;
    auto& inst = utils::RandomGenerator::Instance();
    int invert = inst.GetIntValue(0, 1) ? 1 : -1;
    real_angle = invert * real_angle;
    auto random_angle = inst.GetRealValue(MIN_DELTA_ANGLE, MAX_DELTA_ANGLE);
    return { RocketParams{mRect.mX, mRect.mY, real_angle, new_level, salute_type},
             RocketParams{mRect.mX, mRect.mY, real_angle + random_angle, new_level, salute_type},
             RocketParams{mRect.mX, mRect.mY, real_angle - random_angle, new_level, salute_type} };
}

void Rocket::Move()
{
    /**
    * In this method, the movement of the rocket is calculated as for an object launched at an angle to the horizon.
    * But in addition to the action of gravity, factors such as 
    * air resistance, angular velocity and wind action are also taken into account.
    * From the second law of Newton a = F / m, whence it follows that
    * d^2(x)/dt^2 = dvx/dt = (-(c/m) * vx - (k/m) * vy) * sqrt(vx * vx + vy * vy)
    * d^2(y)/dt^2 = dvy/dt = ((k/m) * vx - (c/m) * vy) * sqrt(vx * vx + vy * vy) - g ,
    * where с = (1/2) * Сd * A * ro, k = (1/2) * Cl * A * ro.
    * c is the drag coefficient due to air resistance,
    * k - offset factor due to angular velocity and wind
    * A - the cross-sectional area of ​​the rocket (or any other object)
    * ro - air resistance
    * Cd - deceleration parameter, Cl - offset parameter
    * Cd = 0.30 + (2.58 * 10^(-4)) * w
    * Cl = 0.319 * (1 - exp(-2.48 * 10^(-3) * w)), where w is the angular velocity in rad / s
    *
    * Further, the RK4 method is used - one of the Runge-Kutta family of numerical methods.
    * At each step n and at time iteration dt, it turns out
    * xy[n+1] = xy[n] + (1/6) * (k1 + 2*k2 + 2*k3 + k4), где
    * k1 = dt * RK4(xy[n])
    * k2 = dt * RK4(xy[n] + k1 / 2)
    * k3 = dt * RK4(xy[n] + k2 / 2)
    * k4 = dt * RK4(xy[n] + k3)
    *
    * xy - at each iteration n, this is an array of 4 elements:
    * current x and y coordinates and velocity projections vx and vy
    */

    if (mIsPaused)
        return;

    // If the rocket did not hit one target,
    // it is considered used when it hits the ground.
    if (mXYold[2] < -0.001)
    {
        mIsUsed = true;
        return;
    }

    float y_new[N_DIM];
    float y1[N_DIM];
    float y2[N_DIM];
    float y3[N_DIM];

    // k1 = f(tn, yn)
    float* k1 = RKFunc(mXYold);
    for (int i = 0; i < N_DIM; i++)
        y1[i] = mXYold[i] + 0.5 * TIME_DELTA * k1[i];

    // k2 = f(tn + h/2, yn + k1/2)
    float* k2 = RKFunc(y1);
    for (int i = 0; i < N_DIM; i++)
        y2[i] = mXYold[i] + 0.5 * TIME_DELTA * k2[i];

    // k3 = f(tn + h/2, yn + k2/2)
    float* k3 = RKFunc(y2);
    for (int i = 0; i < N_DIM; i++)
        y3[i] = mXYold[i] + TIME_DELTA * k3[i];

    // k4 = f(tn + h/2, yn + k3)
    float* k4 = RKFunc(y3);

    for (int i = 0; i < N_DIM; i++)
        y_new[i] = mXYold[i] + TIME_DELTA * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;

    mRect.mX = y_new[0];
    mRect.mY = y_new[2];
    for (int i = 0; i < N_DIM; i++)
        mXYold[i] = y_new[i];

    delete[] k1;
    delete[] k2;
    delete[] k3;
    delete[] k4;
}

void Rocket::SimpleDraw()
{
    if (mIsUsed || !mMainRocket)
        return;

    auto angle = acos(mXYold[3] / sqrt(mXYold[3] * mXYold[3] + mXYold[1] * mXYold[1]));
    float real_angle = angle * PI_DEGREES / M_PI;
    Render::device.PushMatrix();
    Render::device.MatrixTranslate(mRect.mX - mDeltaX, mRect.mY + mDeltaY, 0);
    Render::device.MatrixRotate(math::Vector3(0, 0, 1), real_angle);
    mTexture->Draw();
    Render::device.PopMatrix();
}

void Rocket::Draw()
{
    // Move the rocket on the current iteration
    Move();
    CheckRocketOnUsed();
    SimpleDraw();
}

void Rocket::DrawEffects(EffectsContainer& eff_cont)
{
    if (!mFlyEffect)
        mFlyEffect = eff_cont.AddEffect(FLY_ROCKET_EFFECT);
    
    if (mFlyEffect)
    {
        mFlyEffect->posX = mRect.mX;
        mFlyEffect->posY = mRect.mY;

        if (mIsUsed)
            mFlyEffect->Finish();
    }

    if (mFirstDraw)
    {
        auto shot_effect = eff_cont.AddEffect(SHOT_EFFECT);
        shot_effect->posX = mRect.mX;
        shot_effect->posY = mRect.mY;
        shot_effect->Reset();
        mFirstDraw = false;
    }

    if (!mIsUsed)
        return;

    if (!mSaluteEffect)
        mSaluteEffect = eff_cont.AddEffect(mSaluteEffectName);
    if (!mSaluteEffect)
        return;

    MM::manager.PlaySample("SaluteSound");
    mSaluteEffect->posX = mRect.mX;
    mSaluteEffect->posY = mRect.mY;
    mSaluteEffect->Reset();
}

//------------------------------------------------------------------------------------
// RedRocket

RedRocket::RedRocket(const RocketParams& params)
    : Rocket(params)
{
    InitRocketParams();
}

void RedRocket::InitRocketParams()
{
    mTexture = utils::GetTexture(ROCKET_TEXTURE);
    utils::InitSize(mTexture, mDeltaX, mDeltaY);

    // Convert to rad/s
    float w = ROCKET_RPM * M_PI / 30.00;
    // Drag coefficient
    float CD = 0.30 + 2.58e-4 * w;
    mCm = 0.5 * CD * ROCKET_S * RHO / ROCKET_MASS;

    // Swift factor
    float CL = 0.3187 * (1.0 - exp(-2.483e-3 * w));
    mKm = 0.5 * CL * ROCKET_S * RHO / ROCKET_MASS;
}

//------------------------------------------------------------------------------------
// SaluteGun
SaluteGun::SaluteGun()
    : mIsPaused(false)
{
    mTexture = utils::GetTexture(GUN_TEXTURE);
    IRect gun_rect = mTexture->getBitmapRect();
    mRect.mWidth = gun_rect.width;
    mRect.mHeight = gun_rect.height;

    mWinWidth = Config::WinWidth();
    mRect.mX = mWinWidth / 2 - mRect.mWidth / 2;

    InitRockets(false);
    InitMinMaxPos(0, mWinWidth);
    mShotTimer.Resume();
    mHandShotTimer.Resume();
}

void SaluteGun::Draw()
{
    Render::device.PushMatrix();
    Render::device.MatrixTranslate(mRect.mX, 0, 0);

    IRect gun_rect = mTexture->getBitmapRect();
    FRect rect(gun_rect);
    FRect uv(0, 1, 0, 1);
    mTexture->TranslateUV(rect, uv);
    mTexture->Bind();
    Render::DrawQuad(rect, uv);
    Render::device.PopMatrix();
}

void SaluteGun::InitRockets(bool restart)
{
    mWeaponTimer.Resume();

    if (restart)
    {
        std::for_each(mRocketPool.begin(), mRocketPool.end(), [](RocketPtr& rocket_obj)
        {
            if (rocket_obj->mFlyEffect)
                rocket_obj->mFlyEffect->Finish();
        });
        mRocketPool.clear();
    }
    
    mWeaponTimer.Start();
    mPrevTime = 0.0f;
}

void SaluteGun::InitMinMaxPos(int min, int max)
{
    mMinX = min;
    mMaxX = max - mRect.mWidth;
}

void SaluteGun::Move(bool is_left)
{
    if (is_left)
        mRect.mX -= GUN_VELOCITY;
    else
        mRect.mX += GUN_VELOCITY;
    if (mRect.mX < mMinX)
        mRect.mX = mMinX;
    if (mRect.mX > mMaxX)
        mRect.mX = mMaxX;
}

void SaluteGun::OnPausedMoving(bool pause)
{
    mIsPaused = pause;
    for (auto& rocket : mRocketPool)
        rocket->mIsPaused = pause;
}

void SaluteGun::RocketsDraw(EffectsContainer& eff_cont, const std::string& limit_str)
{
    auto curr_time = mWeaponTimer.getElapsedTime();
    TIME_DELTA = (curr_time - mPrevTime) * 10;
    mPrevTime = curr_time;

    // TODO To acceleration it is necessary to draw salutes in different threads.
    // But there was a problem with the effects container. 
    /*using Future = std::future<void>;
    std::vector<Future> task_list;
    size_t idx = 0;

    auto rocket_draw_func = [](const RocketPtr& rocket, EffectsContainer& eff_cont)
    {
        rocket->Draw();
        rocket->DrawEffects(eff_cont);
    };

    for (auto& rocket : mRocketPool)
    {
        //rocket_draw_func(rocket, eff_cont);
        if (idx == mRocketPool.size() - 1)
            rocket_draw_func(rocket, eff_cont);
        else
        {
            auto task = std::async(rocket_draw_func, std::ref(rocket), std::ref(eff_cont));
            task_list.push_back(std::move(task));
        }

        idx++;
    }

    std::for_each(task_list.begin(), task_list.end(), [](Future& task) {
        task.get();
    });*/

    int limit = utils::lexical_cast<int>(limit_str);
    std::list<RocketPtr> tmp_rocket_pool;
    for (auto& rocket : mRocketPool)
    {
        rocket->Draw();
        rocket->DrawEffects(eff_cont);

        auto new_rockets = rocket->CreateSubRockets(mSaluteEffectName, limit);
        for (auto& params : new_rockets)
            tmp_rocket_pool.push_back(std::make_unique<RedRocket>(params));
    }

    auto delete_func = [](RocketPtr& b_object) -> bool
    {
        return b_object->Clear();
    };

    mRocketPool.remove_if(delete_func);

    if (!tmp_rocket_pool.empty())
        mRocketPool.splice(mRocketPool.end(), tmp_rocket_pool);
}

// Set an effect of the rockets
void SaluteGun::SetEffect(const std::string& effect_name)
{
    mSaluteEffectName = effect_name;
}

bool SaluteGun::MouseShot(int x, int y)
{
    if (mIsPaused || mHandShotTimer.getElapsedTime() < HAND_SHOT_PERIOD)
        return false;

    mHandShotTimer.Resume();
    MM::manager.PlaySample("ShotSound");
    RocketParams main_params(x, y, PI_DEGREES / 2, 0, mSaluteEffectName);
    RocketPtr rocket = std::make_unique<RedRocket>(main_params);

    // Adjusting the initial position of the rocket
    rocket->mFirstDraw = true;
    mRocketPool.push_back(std::move(rocket));
    mHandShotTimer.Start();
    return true;
}

bool SaluteGun::Shot(bool forced)
{
    if (mIsPaused)
        return false;

    if (forced && mHandShotTimer.getElapsedTime() < HAND_SHOT_PERIOD ||
        !forced && mShotTimer.getElapsedTime() < SHOT_PERIOD)
        return false;

    MM::manager.PlaySample("ShotSound");
    mHandShotTimer.Resume();
    mShotTimer.Resume();
    RocketParams main_params(mRect.mX + 2 * mRect.mWidth / 3, 
                             mRect.mHeight, PI_DEGREES / 2, 0, 
                             mSaluteEffectName, true);
    RocketPtr rocket = std::make_unique<RedRocket>(main_params);

    // Adjusting the initial position of the rocket
    rocket->mFirstDraw = true;
    mRocketPool.push_back(std::move(rocket));
    mHandShotTimer.Start();
    mShotTimer.Start();
    return true;
}

}
