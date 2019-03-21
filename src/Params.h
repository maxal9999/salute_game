#pragma once

/**
 * \file
 * \brief File with const params
 * \author Maksimovskiy A.S.
 */

// Add delta x to main button's positions
#define BUTTON_DELTA_POS 15
// Add delta x and y to menu switcher
#define DELTA_X_MENU 15
#define DELTA_Y_MENU 15


// 90 degree angle
extern const float PI_2_DEGREES;
// 180 degree angle
extern const float PI_DEGREES;
// 270 degree angle
extern const float TWO_PI_3_DEGREES;
// 360 degree angle
extern const float TWO_PI_DEGREES;

// Acceleration of gravity
extern const float G;
// Air resistance (kg / m ^ 3)
extern const float RHO;
// Dimension of arrays for the Runge - Kutta formula
constexpr int N_DIM = 4;

// Rocket params
extern const std::string ROCKET_TEXTURE;
extern const std::string ROCKET_TEXTURE_MINI;
extern const int ROCKET_VELOCITY;
// Min and max distances for rocket's fly
extern const int MIN_DISTANCE;
extern const int MAX_DISTANCE;
// Min and max distances for main rocket's fly
extern const int MAIN_MIN_DISTANCE;
extern const int MAIN_MAX_DISTANCE;
// Min and max delta angles for rocket's fly
extern const int MIN_DELTA_ANGLE;
extern const int MAX_DELTA_ANGLE;
// Angular velocity
extern const float ROCKET_RPM;
// Cross sectional area in m ^ 2
extern const float ROCKET_S;
// Rocket mass in kg
extern const float ROCKET_MASS;

// Salute gun params
extern const std::string GUN_TEXTURE;
extern const int GUN_VELOCITY;
extern const float SHOT_PERIOD;

// Component's textures
extern const std::string CURSOR_TEXTURE;
extern const std::string PLAY_ENABLE_TEXTURE;
extern const std::string PLAY_DISABLE_TEXTURE;
extern const std::string PAUSE_ENABLE_TEXTURE;
extern const std::string PAUSE_DISABLE_TEXTURE;
extern const std::string STOP_ENABLE_TEXTURE;
extern const std::string STOP_DISABLE_TEXTURE;
extern const std::string SETTING_ENABLE_TEXTURE;
extern const std::string SETTING_DISABLE_TEXTURE;
extern const std::string LEFT_ENABLE_TEXTURE;
extern const std::string LEFT_DISABLE_TEXTURE;
extern const std::string RIGHT_ENABLE_TEXTURE;
extern const std::string RIGHT_DISABLE_TEXTURE;
extern const std::string SWITCHER_ENABLE_TEXTURE;
extern const std::string SWITCHER_DISABLE_TEXTURE;

// Switcher's names
extern const std::string BACKGROUND_SWITCHER;
extern const std::string DIFFICULTY_SWITCHER;
extern const std::string TYPE_SWITCHER;
extern const std::string CONTINUE_SWITCHER;
extern const std::string EXIT_SWITCHER;

// Effects
extern const std::string FLY_ROCKET_EFFECT;
extern const std::string SHOT_EFFECT;
extern const std::string SALUTE_EFFECT;
extern const std::string SALUTE_TYPE_FORTH;

// Class to get params
class Config
{
public:
    Config() = default;

    using SettingType = std::pair<std::string, std::string>;
    using SettingsType = std::list<SettingType>;

    // Background types
    static SettingsType Backgrounds();

    // Salute difficulty
    static SettingsType Difficulty();

    // Salute types
    static SettingsType SaluteTypes();
    static int SaluteCount();

    // Window's width
    static int WinWidth();

    // Window's height
    static int WinHeight();
};
