#include "stdafx.h"

#include "Components.h"
#include "Utils.h"
#include "Params.h"


namespace components
{

//------------------------------------------------------------------------------------
// Object

Object::Object(const std::string& tex_enable, const std::string& tex_disable)
   : mEnable(false), mShow(true)
{
    mEnableTexture = utils::GetTexture(tex_enable);
    mDisableTexture = utils::GetTexture(tex_disable);
    auto rect = mEnableTexture->getBitmapRect();
    mRect.mHeight = rect.height;
    mRect.mWidth = rect.width;
}

void Object::Action()
{
    if (!mShow)
        return;

    mAction();
}

void Object::ChangeEnable(bool activate)
{
    mEnable = activate;
}

void Object::ChangeShow(bool show)
{
    mShow = show;
}

bool Object::CheckHitOnObject(int x, int y)
{
    return mRect.mX <= x && (mRect.mX + mRect.mWidth) >= x &&
           mRect.mY <= y && (mRect.mY + mRect.mHeight) >= y;
}

void Object::InitAction(std::function<void()> action)
{ 
    mAction = action; 
}

//------------------------------------------------------------------------------------
// Button

Button::Button(const std::string& tex_enable,
               const std::string& tex_disable)
    : Object(tex_enable, tex_disable)
{
}

void Button::Draw()
{
    if (!mShow)
        return;

    Render::device.PushMatrix();
    Render::device.MatrixTranslate(mRect.mX, mRect.mY, 0.0f);
    auto pic_tex = mEnable ? mEnableTexture : mDisableTexture;
    pic_tex->Draw();
    Render::device.PopMatrix();
}

bool Button::OnMouseDown(int x, int y)
{
    if (!mShow || !CheckHitOnObject(x, y))
        return false;

    ChangeEnable(true);
    return true;
}

bool Button::OnMouseUp(int x, int y)
{
    if (!mShow)
        return false;

    ChangeEnable(false);
    if (!CheckHitOnObject(x, y))
        return false;

    Action();
    return true;
}

//------------------------------------------------------------------------------------
// Factory

namespace 
{

std::shared_ptr<Object> CreateButton(const std::string& tex_enable, 
                                     const std::string& tex_disable,
                                     int x, int y)
{
    auto new_button = std::make_shared<Button>(tex_enable, tex_disable);
    new_button->mRect.mX = x;
    new_button->mRect.mY = y;
    return new_button;
}

}

std::shared_ptr<Object> PlayButton(int x, int y)
{
    return CreateButton(PLAY_ENABLE_TEXTURE, PLAY_DISABLE_TEXTURE, x, y);
}

std::shared_ptr<Object> PauseButton(int x, int y)
{
    return CreateButton(PAUSE_ENABLE_TEXTURE, PAUSE_DISABLE_TEXTURE, x, y);
}

std::shared_ptr<Object> StopButton(int x, int y)
{
    return CreateButton(STOP_ENABLE_TEXTURE, STOP_DISABLE_TEXTURE, x, y);
}

std::shared_ptr<Object> SettingsButton(int x, int y)
{
    return CreateButton(SETTING_ENABLE_TEXTURE, SETTING_DISABLE_TEXTURE, x, y);
}

std::shared_ptr<Object> LeftButton(int x, int y)
{
    return CreateButton(LEFT_ENABLE_TEXTURE, LEFT_DISABLE_TEXTURE, x, y);
}

std::shared_ptr<Object> RightButton(int x, int y)
{
    return CreateButton(RIGHT_ENABLE_TEXTURE, RIGHT_DISABLE_TEXTURE, x, y);
}

//------------------------------------------------------------------------------------
// Switcher

Switcher::Switcher(const std::string& name,
                   const std::string& tex_enable,
                   const std::string& tex_disable,
                   const std::shared_ptr<Object>& left,
                   const std::shared_ptr<Object>& right)
    : Object(tex_enable, tex_disable),
    mLeftButton(left),
    mRightButton(right),
    mName(name)
{
    mShow = false;
}

void Switcher::Draw()
{
    if (!mShow)
        return;
    
    Render::device.PushMatrix();
    Render::device.MatrixTranslate(mRect.mX, mRect.mY, 0.0f);
    auto mouse = Core::mainInput.GetMousePos();
    bool is_mouse_hit = CheckHitOnObject(mouse.x, mouse.y);
    auto pic_tex = is_mouse_hit ? mEnableTexture : mDisableTexture;
    pic_tex->Draw();
    Render::device.PopMatrix();

    auto& print_name = is_mouse_hit && !mSettingName.empty() ? mSettingName : mName;
    Render::PrintString(mNameRect.mX, mNameRect.mY, print_name, 2.0f, CenterAlign, CenterAlign);

    if (mLeftButton)
        mLeftButton->Draw();
    if (mRightButton)
        mRightButton->Draw();
}

bool Switcher::OnMouseDown(int x, int y)
{
    bool is_hit = false;
    if (mLeftButton)
        is_hit = mLeftButton->OnMouseDown(x, y);
    if (!is_hit && mRightButton)
        is_hit = mRightButton->OnMouseDown(x, y);
    if (!is_hit && !mLeftButton && !mRightButton && CheckHitOnObject(x, y))
        Action();
    return is_hit;
}

bool Switcher::OnMouseUp(int x, int y)
{
    bool is_hit = false;
    if (mLeftButton)
        is_hit = mLeftButton->OnMouseUp(x, y);
    if (!is_hit && mRightButton)
        is_hit = mRightButton->OnMouseUp(x, y);
    return is_hit;
}

SwitcherPtr NewSwitcher(const std::string& name,
                        const std::shared_ptr<Object>& left,
                        const std::shared_ptr<Object>& right)
{
    return std::make_shared<Switcher>(name, SWITCHER_ENABLE_TEXTURE, SWITCHER_DISABLE_TEXTURE, left, right);
}

//-------------------------------------------------------------------------------------
// ObjectPool
bool ObjectPool::CheckMouseDown(int x, int y)
{
    for (auto& object : mObjectList)
        if (object->OnMouseDown(x, y))
            return true;

    return false;
}

bool ObjectPool::CheckMouseUp(int x, int y)
{
    for (auto& object : mObjectList)
        if (object->OnMouseUp(x, y))
            return true;

    return false;
}

void ObjectPool::DrawAll()
{
    for (auto& object : mObjectList)
        object->Draw();
}

//-------------------------------------------------------------------------------------
// Menu

Menu::Menu(int x, int y)
    : ObjectPool(),
    mDeltaX(DELTA_X_MENU),
    mDeltaY(DELTA_Y_MENU),
    IsActivate(false)
{
    mRect.mX = x;
    mRect.mY = y;
}

void Menu::AddSwitcher(const SwitcherPtr& switch_obj)
{
    mObjectList.push_back(switch_obj);
    auto width = switch_obj->mRect.mWidth;
    if (width > mRect.mWidth)
        mRect.mWidth = width + 2 * mDeltaX;
    
    auto height = switch_obj->mRect.mHeight;
    auto delta_h = mDeltaY + height;
    mRect.mHeight += delta_h;
    mRect.mY -= delta_h;
    
    // Init position of the switcher
    switch_obj->mRect.mX = mRect.mX - switch_obj->mRect.mWidth / 2;
    switch_obj->mRect.mY = mRect.mY + delta_h + switch_obj->mRect.mHeight / 2;
    int switch_x = switch_obj->mRect.mX;
    int switch_y = switch_obj->mRect.mY;

    // Init position of the name
    switch_obj->mNameRect.mX = switch_obj->mRect.mX + switch_obj->mRect.mWidth / 2;
    switch_obj->mNameRect.mY = switch_obj->mRect.mY + switch_obj->mRect.mHeight / 2;

    auto& left = switch_obj->mLeftButton;
    if (left)
    {
        left->mRect.mX = switch_x + left->mRect.mWidth / 2;
        left->mRect.mY = switch_y + left->mRect.mHeight / 2;
    }

    auto& right = switch_obj->mRightButton;
    if (left)
    {
        right->mRect.mX = switch_x + switch_obj->mRect.mWidth - 1.5 * right->mRect.mWidth;
        right->mRect.mY = switch_y + right->mRect.mHeight / 2;
    }
}

void Menu::AddSwitchers(std::initializer_list<SwitcherPtr> switchers)
{
    for (auto& switcher : switchers)
        AddSwitcher(switcher);
}

void Menu::Show(bool activate)
{
    IsActivate = !IsActivate;
    for (auto& switcher : mObjectList)
        switcher->ChangeShow(activate);
}

//-------------------------------------------------------------------------------------
// ObjectPool

void ButtonPool::AddObject(const ObjectPtr& object)
{
    mObjectList.push_back(object);
}

void ButtonPool::AddObjects(std::initializer_list<ObjectPtr> objects)
{
    for (auto& object : objects)
        AddObject(object);
}

//-------------------------------------------------------------------------------------
// Cursor

void Cursor::Draw()
{
    static auto CURSOR = utils::GetTexture(CURSOR_TEXTURE);

    // Current position of the mouse. The cursor is attached to the mouse cursor.
    auto mouse_pos = Core::mainInput.GetMousePos();
    static IRect CURSOR_RECT = CURSOR->getBitmapRect();
    Render::device.PushMatrix();
    Render::device.MatrixTranslate(mouse_pos.x, mouse_pos.y - CURSOR_RECT.height, 0.0f);
    CURSOR->Draw();
    Render::device.PopMatrix();
}

void Cursor::InitAction(std::function<void(int, int)> action)
{
    mAction = action;
}

void Cursor::Action(int x, int y)
{
    mAction(x, y);
}

}
