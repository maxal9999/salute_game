#pragma once

/**
 * \file
 * \brief File with const params
 * \author Maksimovskiy A.S.
 */

#include "stdafx.h"
#include "Params.h"
#include <Windows.h>


// 90 degree angle
const float PI_2_DEGREES = 90.0f;
// 180 degree angle
const float PI_DEGREES = 180.0f;
// 270 degree angle
const float TWO_PI_3_DEGREES = 270.0f;
// 360 degree angle
const float TWO_PI_DEGREES = 360.0f;

// Acceleration of gravity
const float G = 9.81f;
// Air resistance (kg / m ^ 3)
const float RHO = 1.23;

// Rocket params
const std::string ROCKET_TEXTURE = "RedRocket";
const int ROCKET_VELOCITY = 135;
// Min and max distances for rocket's fly
const int MIN_DISTANCE = 200;
const int MAX_DISTANCE = 400;
// Min and max distances for main rocket's fly
const int MAIN_MIN_DISTANCE = 600;
const int MAIN_MAX_DISTANCE = 750;
// Min and max delta angles for rocket's fly
const int MIN_DELTA_ANGLE = 90;
const int MAX_DELTA_ANGLE = 150;
// Angular velocity
const float ROCKET_RPM = 150.0f;
// Cross sectional area in m ^ 2
const float ROCKET_S = 0.0016f;
// Rocket mass in kg
const float ROCKET_MASS = 5.0f;

// Salute gun params
const std::string GUN_TEXTURE = "SaluteGun";
const int GUN_VELOCITY = 30;
const float HAND_SHOT_PERIOD = 0.5f;
const float SHOT_PERIOD = 5.0f;

// Component's textures
const std::string CURSOR_TEXTURE = "Cursor";
const std::string PLAY_ENABLE_TEXTURE = "PlayEnable";
const std::string PLAY_DISABLE_TEXTURE = "PlayDisable";
const std::string PAUSE_ENABLE_TEXTURE = "PauseEnable";
const std::string PAUSE_DISABLE_TEXTURE = "PauseDisable";
const std::string STOP_ENABLE_TEXTURE = "StopEnable";
const std::string STOP_DISABLE_TEXTURE = "StopDisable";
const std::string SETTING_ENABLE_TEXTURE = "SettingsEnable";
const std::string SETTING_DISABLE_TEXTURE = "SettingsDisable";
const std::string LEFT_ENABLE_TEXTURE = "LeftEnable";
const std::string LEFT_DISABLE_TEXTURE = "LeftDisable";
const std::string RIGHT_ENABLE_TEXTURE = "RightEnable";
const std::string RIGHT_DISABLE_TEXTURE = "RightDisable";
const std::string SWITCHER_ENABLE_TEXTURE = "SwitcherEnable";
const std::string SWITCHER_DISABLE_TEXTURE = "SwitcherDisable";

// Switcher's names
const std::string BACKGROUND_SWITCHER = "Background";
const std::string DIFFICULTY_SWITCHER = "Difficulty";
const std::string TYPE_SWITCHER = "Salute type";
const std::string CONTINUE_SWITCHER = "Continue";
const std::string EXIT_SWITCHER = "Exit";

// Effects
const std::string FLY_ROCKET_EFFECT = "FlyRocket";
const std::string SHOT_EFFECT = "Shot";
const std::string SALUTE_EFFECT = "Salute";

// Backgrounds const
const std::string BACKGROUND_FIRST = "Background1";
const std::string BACKGROUND_FIRST_NAME = "Sidney";
const std::string BACKGROUND_SECOND = "Background2";
const std::string BACKGROUND_SECOND_NAME = "New York";
const std::string BACKGROUND_THIRD = "Background3";
const std::string BACKGROUND_THIRD_NAME = "London";

// Difficulty const
const std::string DIFFICULTY_FIRST = "0";
const std::string DIFFICULTY_FIRST_NAME = "Simple";
const std::string DIFFICULTY_SECOND = "1";
const std::string DIFFICULTY_SECOND_NAME = "Middle";
const std::string DIFFICULTY_THIRD = "2";
const std::string DIFFICULTY_THIRD_NAME = "Hard";
const std::string DIFFICULTY_FORTH = "3";
const std::string DIFFICULTY_FORTH_NAME = "Apocalypse";

// Salute types const
const std::string SALUTE_TYPE_FIRST = "Salute1";
const std::string SALUTE_TYPE_FIRST_NAME = "Stars scatter";
const std::string SALUTE_TYPE_SECOND = "Salute2";
const std::string SALUTE_TYPE_SECOND_NAME = "Rain fall";
const std::string SALUTE_TYPE_THIRD = "Salute3";
const std::string SALUTE_TYPE_THIRD_NAME = "Rainbow glow";
const std::string SALUTE_TYPE_FORTH = "Mix";
const std::string SALUTE_TYPE_FORTH_NAME = "Salute mixed";

// Background types
Config::SettingsType Config::Backgrounds()
{
    // Backgrounds type
    auto first_groud = std::make_pair(BACKGROUND_FIRST, BACKGROUND_FIRST_NAME);
    auto second_groud = std::make_pair(BACKGROUND_SECOND, BACKGROUND_SECOND_NAME);
    auto third_groud = std::make_pair(BACKGROUND_THIRD, BACKGROUND_THIRD_NAME);
    return { first_groud, second_groud, third_groud };
}

// Salute difficulty
Config::SettingsType Config::Difficulty()
{
    auto first_level = std::make_pair(DIFFICULTY_FIRST, DIFFICULTY_FIRST_NAME);
    auto second_level = std::make_pair(DIFFICULTY_SECOND, DIFFICULTY_SECOND_NAME);
    auto third_level = std::make_pair(DIFFICULTY_THIRD, DIFFICULTY_THIRD_NAME);
    auto forth_level = std::make_pair(DIFFICULTY_FORTH, DIFFICULTY_FORTH_NAME);
    return { first_level, second_level, third_level, forth_level };
}

// Salute types
Config::SettingsType Config::SaluteTypes()
{
    auto first_type = std::make_pair(SALUTE_TYPE_FIRST, SALUTE_TYPE_FIRST_NAME);
    auto second_type = std::make_pair(SALUTE_TYPE_SECOND, SALUTE_TYPE_SECOND_NAME);
    auto third_type = std::make_pair(SALUTE_TYPE_THIRD, SALUTE_TYPE_THIRD_NAME);
    auto forth_type = std::make_pair(SALUTE_TYPE_FORTH, SALUTE_TYPE_FORTH_NAME);
    static auto types = { first_type, second_type, third_type, forth_type };
    return types;
}

int Config::SaluteCount()
{
    static int count = SaluteTypes().size() - 1;
    return count;
}

int Config::WinWidth()
{
    static auto x_screen = GetSystemMetrics(SM_CXSCREEN);
    return x_screen;
}

int Config::WinHeight()
{
    static auto y_screen = GetSystemMetrics(SM_CYSCREEN);
    return y_screen;
}
