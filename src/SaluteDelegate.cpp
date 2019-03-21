#include "stdafx.h"
#include "Params.h"
#include "SaluteDelegate.h"
#include "SaluteWidget.h"


void SaluteDelegate::GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height)
{
    width = Config::WinWidth();
    height = Config::WinHeight();
}

void SaluteDelegate::ScreenMode(DeviceMode &mode)
{
    mode = DeviceMode::Windowed;
}

void SaluteDelegate::RegisterTypes()
{
    REGISTER_WIDGET_XML(SaluteWidget, "SaluteWidget");
}

void SaluteDelegate::LoadResources()
{
    Core::LuaExecuteStartupScript("start.lua");
}

void SaluteDelegate::OnResourceLoaded() {
    if (Core::appInstance->AllResourcesLoaded()) {
        Core::appInstance->SetLoaded();
    }
}

void SaluteDelegate::OnPostDraw() 
{
    if (!Render::isFontLoaded("arial"))
        return;

    Render::BindFont("arial");
}
