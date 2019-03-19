#pragma once

#include "Components.h"
#include "SaluteGun.h"


// Widget for drawing the battlefield.
class SaluteWidget : public GUI::Widget
{
public:
    SaluteWidget(const std::string& name, rapidxml::xml_node<>* elem);
    
    void Draw() override;
    void Update(float dt) override;
    	
    void AcceptMessage(const Message& message) override;
    	
    bool MouseDown(const IPoint& mouse_pos) override;
    void MouseMove(const IPoint& mouse_pos) override;
    void MouseUp(const IPoint& mouse_pos) override;
    
    void KeyPressed(int keyCode) override;
    void CharPressed(int unicodeChar) override;

private:
    void Init();
	void InitButtons();
	void InitMenu();

	// Background
	using SettingName = std::pair<std::string, std::string>;
	utils::RecursiveList<SettingName> mBackGrounds;
	// All buttons
	components::ButtonPool mButtonPool;
	// Objects for drawing effects
	EffectsContainer mEffCont;
	// Menu with switchers
	components::Menu mMenu;
	// Salute gun for shot rockets
	weapons::SaluteGun mSaluteGun;
	// Salute difficulty
	utils::RecursiveList<SettingName> mSaluteDifficulty;
    // Timer
    Core::Timer mTimer;

};
