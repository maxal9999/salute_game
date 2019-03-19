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

// 180 degree angle
constexpr float PI_DEGREES = 180.0f;
// Acceleration of gravity
constexpr float G = 9.81f;
// Air resistance (kg / m ^ 3)
constexpr float RHO = 1.23;
// Dimension of arrays for the Runge - Kutta formula
constexpr int N_DIM = 4;

// Rocket params
const std::string ROCKET_TEXTURE = "RedRocket";
constexpr int ROCKET_VELOCITY = 125;
// Angular velocity
constexpr float ROCKET_RPM = 5.0f;
// Cross sectional area in m ^ 2
constexpr float ROCKET_S = 0.0016f;
// Rocket mass in kg
constexpr float ROCKET_MASS = 5.0f;

// Salute gun params
const std::string GUN_TEXTURE = "SaluteGun";
constexpr int GUN_VELOCITY = 30;

// Effects
const std::string FLY_ROCKET_EFFECT = "FlyRocket";
const std::string SHOT_EFFECT = "Shot";
const std::string SALUTE_EFFECT = "Salute";

// Class to get params
class Config
{
public:
	Config() = default;

	// Window's width
	static int WinWidth();

	// Window's height
	static int WinHeight();
};