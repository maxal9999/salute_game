#include "stdafx.h"

#include "Utils.h"


namespace utils
{

//------------------------------------------------------------------------------------

void DrawTexture(const std::string& texture)
{
   Render::Texture* win = Core::resourceManager.Get<Render::Texture>(texture);
   Render::device.PushMatrix();
   win->Draw();
   Render::device.PopMatrix();
}

//------------------------------------------------------------------------------------

Rect::Rect()
   : mX(0), mY(0), mHeight(0), mWidth(0)
{
}

Rect::Rect(int x, int y, int height, int width)
   : mX(x), mY(y), mHeight(height), mWidth(width)
{
}

}
