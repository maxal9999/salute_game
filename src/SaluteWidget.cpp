#include "stdafx.h"

#include <windows.h>

#include "ClassHelpers.h"
#include "Params.h"
#include "SaluteWidget.h"


using ObjectPtr = std::shared_ptr<components::Object>;

SaluteWidget::SaluteWidget(const std::string& name, rapidxml::xml_node<>* elem)
    : Widget(name),
	mMenu(Config::WinWidth() / 2, Config::WinHeight() / 2)
{
    Init();
}

void SaluteWidget::Init()
{
	// Init backgrounds type
	auto first_groud = std::make_pair("Background1", "Sidney");
	auto second_groud = std::make_pair("Background2", "New York");
	auto third_groud = std::make_pair("Background3", "London");
	mBackGrounds.InitList({ first_groud, second_groud, third_groud });

	// Init difficulty level
	auto first_level = std::make_pair("0", "Simple");
	auto second_level = std::make_pair("1", "Middle");
	auto third_level = std::make_pair("2", "Hard");
	auto forth_level = std::make_pair("3", "Apocalypse");
	mSaluteDifficulty.InitList({ first_level, second_level, third_level, forth_level });
	
	InitButtons();
	InitMenu();

    mTimer.Start();
#if defined(ENGINE_TARGET_WIN32)
    //ShowCursor(FALSE);
#endif
}

void SaluteWidget::InitButtons()
{
	int x_pos = BUTTON_DELTA_POS;

	// Pause button
	auto pause_button = components::PauseButton(BUTTON_DELTA_POS, BUTTON_DELTA_POS);

	auto salute = &mSaluteGun;

	// Play button
	auto play_button = components::PlayButton(BUTTON_DELTA_POS, BUTTON_DELTA_POS);
	play_button->InitAction([salute, play_button, pause_button]()
	{
		salute->OnPausedMoving(false);
		play_button->ChangeShow(false);
		pause_button->ChangeShow(true);
	});
	play_button->ChangeShow(false);

	// Pause button
	pause_button->InitAction([salute, play_button, pause_button]()
	{
		salute->OnPausedMoving(true);
		play_button->ChangeShow(true);
		pause_button->ChangeShow(false);
	});
	x_pos += pause_button->mRect.mWidth + BUTTON_DELTA_POS;

	// Stop button
	auto stop_button = components::StopButton(x_pos, BUTTON_DELTA_POS);
	stop_button->InitAction([salute]()
	{
		salute->InitRockets(true);
	});
	x_pos += stop_button->mRect.mWidth + BUTTON_DELTA_POS;

	// Settings button
	auto menu_ptr = &mMenu;
	auto set_button = components::SettingsButton(x_pos, BUTTON_DELTA_POS);
	set_button->InitAction([menu_ptr]()
	{
		menu_ptr->Show(!menu_ptr->IsActivate);
	});

	mButtonPool.AddObjects({ play_button, pause_button, stop_button, set_button });
}

void SaluteWidget::InitMenu()
{
	// Background switcher
	auto ground_left = components::LeftButton(0, 0);
	auto ground_ptr = &mBackGrounds;
	auto ground_right = components::RightButton(0, 0);
	auto ground_switcher = components::NewSwitcher("Background", ground_left, ground_right);
	ground_switcher->SetSettingName(mBackGrounds.Value().second);
	ground_left->InitAction([ground_switcher, ground_ptr]()
	{
		ground_ptr->Prev();
		ground_switcher->SetSettingName(ground_ptr->Value().second);
	});
	ground_right->InitAction([ground_switcher, ground_ptr]()
	{
		ground_ptr->Next();
		ground_switcher->SetSettingName(ground_ptr->Value().second);
	});

	// Change difficulty of the salute
	auto mode_left = components::LeftButton(0, 0);
	auto mode_ptr = &mSaluteDifficulty;
	auto mode_right = components::RightButton(0, 0);
	auto mode_switcher = components::NewSwitcher("Difficulty", mode_left, mode_right);
	mode_switcher->SetSettingName(mSaluteDifficulty.Value().second);
	mode_left->InitAction([mode_switcher, mode_ptr]()
	{
		mode_ptr->Prev();
		mode_switcher->SetSettingName(mode_ptr->Value().second);
	});
	mode_right->InitAction([mode_switcher, mode_ptr]()
	{
		mode_ptr->Next();
		mode_switcher->SetSettingName(mode_ptr->Value().second);
	});
	
	// Change color of the salute
	auto color_left = components::LeftButton(0, 0);
	auto color_right = components::RightButton(0, 0);
	auto color_switcher = components::NewSwitcher("Color", color_left, color_right);

	// Continue switcher
	auto menu_ptr = &mMenu;
	auto continue_switcher = components::NewSwitcher("Continue");
	continue_switcher->InitAction([menu_ptr]()
	{
		menu_ptr->Show(false);
	});

	// Exit switcher
	auto exit_switcher = components::NewSwitcher("Exit");
	exit_switcher->InitAction([]()
	{
		exit(0);
	});

	mMenu.AddSwitchers({ ground_switcher, mode_switcher, color_switcher, continue_switcher, exit_switcher });
}

void SaluteWidget::Draw()
{
	// Background draw
	utils::DrawTexture(mBackGrounds.Value().first);
	// Draw all buttons
	mButtonPool.DrawAll();
	// Draw salute and rockets
	mSaluteGun.Draw();
	mSaluteGun.RocketsDraw(mEffCont, mSaluteDifficulty.Value().first);
    // Draw all the effects that are added to the container
    mEffCont.Draw();
	// Draw menu with switchers
	mMenu.DrawAll();
}

void SaluteWidget::Update(float dt)
{
    mEffCont.Update(dt);
}

bool SaluteWidget::MouseDown(const IPoint &mouse_pos)
{
	mButtonPool.CheckMouseDown(mouse_pos.x, mouse_pos.y);
	mMenu.CheckMouseDown(mouse_pos.x, mouse_pos.y);
    if (Core::mainInput.GetMouseRightButton())
    {
    }
    else
    {
    }
    return false;
}

void SaluteWidget::MouseMove(const IPoint &mouse_pos)
{
}

void SaluteWidget::MouseUp(const IPoint &mouse_pos)
{
	mButtonPool.CheckMouseUp(mouse_pos.x, mouse_pos.y);
	mMenu.CheckMouseUp(mouse_pos.x, mouse_pos.y);
}

void SaluteWidget::AcceptMessage(const Message& message)
{
}

void SaluteWidget::KeyPressed(int keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		mSaluteGun.Move();
		break;
	case VK_RIGHT:
		mSaluteGun.Move(false);
		break;
	case VK_SPACE:
		mSaluteGun.Shot();
		break;
	}
}

void SaluteWidget::CharPressed(int unicodeChar)
{
}
