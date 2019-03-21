#pragma once

/**
 * \file
 * \brief Base service components of the game
 * \author Maksimovskiy A.S.
 */

#include <functional>
#include <list>
#include <memory>
#include <string>

#include "Utils.h"


namespace components
{

// Class for describe all service objects in the game.
class Object
{
public:
    Object(const std::string& tex_enable,
           const std::string& tex_disable);
    virtual ~Object() = default;
    
    // Perform action
    void Action();
    // Put object into active mode
    void ChangeEnable(bool activate);
    // Put object into show mode
    void ChangeShow(bool show);
    // Check that the mouse click was on the object
    bool CheckHitOnObject(int x, int y);
    // Drawing object
    virtual void Draw() = 0;
    // Init action on this object
    void InitAction(std::function<void()> action);
    // Performing actions if the mouse down on an object
    virtual bool OnMouseDown(int x, int y) = 0;
    // Performing actions if the mouse up on an object
    virtual bool OnMouseUp(int x, int y) = 0;

    // Texture in active mode
    Render::Texture* mEnableTexture;
    // Texture in inactive mode
    Render::Texture* mDisableTexture;

    // Action of the button
    std::function<void()> mAction;
    // Active mode flag
    bool mEnable;
    // Object size
    utils::Rect mRect;
    // Show mode flag
    bool mShow;
};

//-------------------------------------------------------------------------------------
// Class for work with button.
struct Button : public Object
{
    Button(const std::string& tex_enable,
           const std::string& tex_disable);

    virtual ~Button() = default;
    
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

    virtual ~Switcher() = default;
    
    void Draw() override;
    bool OnMouseDown(int x, int y) override;
    bool OnMouseUp(int x, int y) override;
    // Set the switcher's name in active mode
    void SetSettingName(const std::string& set_name) { mSettingName = set_name; }

    // Smart pointer to the left button of switcher
    std::shared_ptr<Object> mLeftButton;
    // Smart pointer to the right button of switcher
    std::shared_ptr<Object> mRightButton;
    // Size and position of the switcher name
    utils::Rect mNameRect;

private:
    // Switcher's name in inactive mode
    std::string mName;
    // Switcher's name in active mode
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
    ~ObjectPool() = default;

    // Performing actions if the mouse down on one of the objects
    bool CheckMouseDown(int x, int y);
    // Performing actions if the mouse up on one of the objects
    bool CheckMouseUp(int x, int y);
    // Drawing all objects
    void DrawAll();

    // List of the objects
    std::list<ObjectPtr> mObjectList;
};

//-------------------------------------------------------------------------------------
// Class for menu control.
class Menu : public ObjectPool
{
public:
    Menu(int x, int y);
    ~Menu() = default;

    // Add new switch on the menu panel
    void AddSwitcher(const SwitcherPtr& switch_obj);
    // Add switches on the menu panel
    void AddSwitchers(std::initializer_list<SwitcherPtr> switchers);
    // Show the menu panel
    void Show(bool activate);

    // Active mode flag
    bool IsActivate;

private:
    // Corrective parameters
    int mDeltaX;
    int mDeltaY;
    
    // Menu size
    utils::Rect mRect;
};

//-------------------------------------------------------------------------------------
// Class to control all buttons.
class ButtonPool : public ObjectPool
{
public:
    ButtonPool() = default;
    ~ButtonPool() = default;
    
    // Add button in container
    void AddObject(const ObjectPtr& object);
    // Add buttons in container
    void AddObjects(std::initializer_list<ObjectPtr> objects);
};

//-------------------------------------------------------------------------------------
// Class to draw cursor.
struct Cursor
{
    // Start salute in (x, y) position
    void Action(int x, int y);

    // Drawing cursor
    void Draw();

    // Init action
    void InitAction(std::function<void(int, int)> action);

private:
    // Function to start salute
    std::function<void(int, int)> mAction;
};

}
