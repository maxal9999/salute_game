#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <string>

#include "Utils.h"


namespace components
{

class Object
{
public:
    Object(const std::string& tex_enable,
           const std::string& tex_disable);
    
    virtual void Action();
    virtual void ChangeEnable(bool activate);
    virtual void ChangeShow(bool show);
    virtual bool CheckHitOnObject(int x, int y);
    virtual void Draw() = 0;
    virtual void InitAction(std::function<void()> action);
    virtual bool OnMouseDown(int x, int y) = 0;
    virtual bool OnMouseUp(int x, int y) = 0;

    Render::Texture* mEnableTexture;
    Render::Texture* mDisableTexture;

    std::function<void()> mAction;
    std::atomic<bool> mEnable;
    utils::Rect mRect;
    std::atomic<bool> mShow;
};

//-------------------------------------------------------------------------------------
// Class for work with button.
struct Button : public Object
{
    Button(const std::string& tex_enable,
           const std::string& tex_disable);
    
    void Draw() override;
    bool OnMouseDown(int x, int y) override;
    bool OnMouseUp(int x, int y) override;
};

// Factory
std::shared_ptr<Object> PlayButton(int x, int y);
std::shared_ptr<Object> PauseButton(int x, int y);
std::shared_ptr<Object> StopButton(int x, int y);
std::shared_ptr<Object> SettingsButton(int x, int y);
std::shared_ptr<Object> LeftButton(int x, int y);
std::shared_ptr<Object> RightButton(int x, int y);

//-------------------------------------------------------------------------------------
// Class for switch options.
struct Switcher : public Object
{
    Switcher() = default;
    Switcher(const std::string& name,
             const std::string& tex_enable,
             const std::string& tex_disable,
             const std::shared_ptr<Object>& left,
             const std::shared_ptr<Object>& right);
    
    void Draw() override;
    bool OnMouseDown(int x, int y) override;
    bool OnMouseUp(int x, int y) override;
    void SetSettingName(const std::string& set_name) { mSettingName = set_name; }

    std::shared_ptr<Object> mLeftButton;
    std::shared_ptr<Object> mRightButton;
    utils::Rect mNameRect;

private:
    std::string mName;
    std::string mSettingName;
};

using SwitcherPtr = std::shared_ptr<Switcher>;

// Factory
SwitcherPtr NewSwitcher(const std::string& name, 
                        const std::shared_ptr<Object>& left = nullptr,
                        const std::shared_ptr<Object>& right = nullptr);

//-------------------------------------------------------------------------------------

using ObjectPtr = std::shared_ptr<Object>;

// Class to control all objects.
class ObjectPool
{
public:
    ObjectPool() = default;

    virtual bool CheckMouseDown(int x, int y);
    virtual bool CheckMouseUp(int x, int y);
    virtual void DrawAll();

    std::list<ObjectPtr> mObjectList;
};

//-------------------------------------------------------------------------------------
// Class for menu control.
class Menu : public ObjectPool
{
public:
    Menu(int x, int y);

    void AddSwitcher(const SwitcherPtr& switch_obj);
    void AddSwitchers(std::initializer_list<SwitcherPtr> switchers);
    void Show(bool activate);

    bool IsActivate;

private:
    int mDeltaX;
    int mDeltaY;
    
    utils::Rect mRect;
};

//-------------------------------------------------------------------------------------
// Class to control all objects.
class ButtonPool : public ObjectPool
{
public:
    ButtonPool() = default;
    
    void AddObject(const ObjectPtr& object);
    void AddObjects(std::initializer_list<ObjectPtr> objects);
};

//-------------------------------------------------------------------------------------
// Class to draw cursor.
struct Cursor
{
    void Action(int x, int y);

    void Draw();

    void InitAction(std::function<void(int, int)> action);

private:
    std::function<void(int, int)> mAction;
};

}
