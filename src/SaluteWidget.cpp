#include "stdafx.h"

#include <windows.h>

#include "Utils.h"
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
	mBackGrounds.InitList(Config::Backgrounds());
	// Init difficulty level
	mSaluteDifficulty.InitList(Config::Difficulty());
	// Init salute types
	mSaluteTypes.InitList(Config::SaluteTypes());
	mSaluteGun.SetEffect(mSaluteTypes.Value().first);
	// Init all buttons
	int x_pos = InitButtons();
	mSaluteGun.InitMinMaxPos(x_pos, Config::WinWidth());
	// Init menu with switchers
	InitMenu();

	// Init action for mouse cursor
	auto salute_gun_ptr = &mSaluteGun;
	mCursor.InitAction([salute_gun_ptr](int x, int y)
	{
		salute_gun_ptr->MouseShot(x, y);
	});

#if defined(ENGINE_TARGET_WIN32)
    ShowCursor(FALSE);
#endif
}

int SaluteWidget::InitButtons()
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
	x_pos += set_button->mRect.mWidth + BUTTON_DELTA_POS;

	mButtonPool.AddObjects({ play_button, pause_button, stop_button, set_button });
	return x_pos;
}

void SaluteWidget::InitMenu()
{
	// Background switcher
	auto ground_left = components::LeftButton(0, 0);
	auto ground_ptr = &mBackGrounds;
	auto ground_right = components::RightButton(0, 0);
	auto ground_switcher = components::NewSwitcher(BACKGROUND_SWITCHER, ground_left, ground_right);
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
	auto mode_switcher = components::NewSwitcher(DIFFICULTY_SWITCHER, mode_left, mode_right);
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
	
	// Change type of the salute
	auto type_left = components::LeftButton(0, 0);
	auto type_right = components::RightButton(0, 0);
	auto type_ptr = &mSaluteTypes;
	auto salute_gun_ptr = &mSaluteGun;
	auto type_switcher = components::NewSwitcher(TYPE_SWITCHER, type_left, type_right);
	type_switcher->SetSettingName(mSaluteTypes.Value().second);
	type_left->InitAction([type_switcher, type_ptr, salute_gun_ptr]()
	{
		type_ptr->Prev();
		type_switcher->SetSettingName(type_ptr->Value().second);
		salute_gun_ptr->SetEffect(type_ptr->Value().first);
	});
	type_right->InitAction([type_switcher, type_ptr, salute_gun_ptr]()
	{
		type_ptr->Next();
		type_switcher->SetSettingName(type_ptr->Value().second);
		salute_gun_ptr->SetEffect(type_ptr->Value().first);
	});

	// Continue switcher
	auto menu_ptr = &mMenu;
	auto continue_switcher = components::NewSwitcher(CONTINUE_SWITCHER);
	continue_switcher->InitAction([menu_ptr]()
	{
		menu_ptr->Show(false);
	});

	// Exit switcher
	auto exit_switcher = components::NewSwitcher(EXIT_SWITCHER);
	exit_switcher->InitAction([]()
	{
		::TerminateProcess(::GetCurrentProcess(), 0);
	});

	mMenu.AddSwitchers({ ground_switcher, mode_switcher, type_switcher, continue_switcher, exit_switcher });
}

void SaluteWidget::Draw()
{
	// Background draw
	utils::DrawTexture(mBackGrounds.Value().first);
	// Draw all buttons
	mButtonPool.DrawAll();
	// Draw salute and rockets
	mSaluteGun.Shot();
	mSaluteGun.Draw();
	mSaluteGun.RocketsDraw(mEffCont, mSaluteDifficulty.Value().first);
    // Draw all the effects that are added to the container
    mEffCont.Draw();
	// Draw menu with switchers
	mMenu.DrawAll();
	// Draw cursor over all objects
	mCursor.Draw();
}

void SaluteWidget::Update(float dt)
{
	mCursor.Draw();
    mEffCont.Update(dt);
}

bool SaluteWidget::MouseDown(const IPoint& mouse_pos)
{
	if (mButtonPool.CheckMouseDown(mouse_pos.x, mouse_pos.y))
		return false;

	if (mMenu.CheckMouseDown(mouse_pos.x, mouse_pos.y))
		return false;

	mCursor.Action(mouse_pos.x, mouse_pos.y);
    return false;
}

void SaluteWidget::MouseMove(const IPoint& mouse_pos)
{
}

void SaluteWidget::MouseUp(const IPoint &mouse_pos)
{
	if (mButtonPool.CheckMouseUp(mouse_pos.x, mouse_pos.y))
		return;

	if (mMenu.CheckMouseUp(mouse_pos.x, mouse_pos.y))
		return;
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
		mSaluteGun.Shot(true);
		break;
	case VK_ESCAPE:
		mMenu.Show(true);
		break;
	}
}

void SaluteWidget::CharPressed(int unicodeChar)
{
}
