#include "stdafx.h"

#include "Utils.h"

#include <ctime>
#include <corecrt_math_defines.h>


namespace utils
{

RandomGenerator::RandomGenerator() :
	mGen(time(0))
{
}

RandomGenerator& RandomGenerator::Instance()
{
	static RandomGenerator rand_gen_instance;
	return rand_gen_instance;
}

int RandomGenerator::GetIntValue(int min, int max)
{
	std::uniform_int_distribution<> urd(min, max);
	return urd(mGen);
}

float RandomGenerator::GetRealValue(int min, int max)
{
	std::uniform_real_distribution<> urd(min, max);
	return static_cast<float>(urd(mGen));
}

//------------------------------------------------------------------------------------

CosSinCalc::CosSinCalc()
{
	for (int i = 0; i < 360; i++)
	{
		mCos.emplace(i, math::cos(i * M_PI / 180));
		mSin.emplace(i, math::sin(i * M_PI / 180));
	}
}

CosSinCalc& CosSinCalc::Instance()
{
	static CosSinCalc cos_sin_instance;
	return cos_sin_instance;
}

void CosSinCalc::CorrectAngle(int& angle)
{
	if (angle < 0)
		angle += 360;
	if (angle >= 360)
		angle -= 360;
}

float CosSinCalc::Cos(int angle)
{
	CorrectAngle(angle);
	auto find_id = mCos.find(angle);
	if (find_id == mCos.end())
		return 0;

	return (*find_id).second;
}

float CosSinCalc::Sin(int angle)
{
	CorrectAngle(angle);
	auto find_id = mSin.find(angle);
	if (find_id == mSin.end())
		return 0;

	return (*find_id).second;
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

//------------------------------------------------------------------------------------

void DrawTexture(const std::string& texture)
{
	Render::Texture* win = Core::resourceManager.Get<Render::Texture>(texture);
	Render::device.PushMatrix();
	win->Draw();
	Render::device.PopMatrix();
}

//------------------------------------------------------------------------------------

int InitSize(Render::Texture* tex, float& delta_x, float& delta_y)
{
	// Getting a rectangle corresponding to the size of the texture
	IRect gun_rect = tex->getBitmapRect();
	int width = gun_rect.width;
	int height = gun_rect.height;

	delta_x = width * 0.5f;
	delta_y = height * 0.5f;

	// According to the Pythagorean theorem, the approximate radius  of a circle describing the texture is calculated
	return static_cast<int>(sqrt(delta_x * delta_x + delta_y * delta_y));
}

//------------------------------------------------------------------------------------

float VectorsAngle(float x1, float x2, float y1, float y2)
{
	float num = x1 * x2 + y1 * y2;
	float denom = sqrt(static_cast<double>(x1 * x1 + y1 * y1) * sqrt(static_cast<double>(x2 * x2 + y2 * y2)));
	return acos(num / denom);
}

//------------------------------------------------------------------------------------

Render::Texture* GetTexture(const std::string& name)
{
	return Core::resourceManager.Get<Render::Texture>(name);
}

}
