#pragma once
#include <Windows.h>
#include <xstring>

namespace Gdiplus
{
	class Bitmap;
	class Graphics;
}

class UpImageUtility
{
public:
	const static wchar_t* FILE_TYPE_JPG;
	const static wchar_t* FILE_TYPE_BMP ;
	const static wchar_t* FILE_TYPE_PNG;

	const static int DisplayImageWidth = 2560;
	const static int DisplayImageHeight = 1600;

	static void Draw9Grid(Gdiplus::Bitmap* bitmap, Gdiplus::Graphics* gs, RECT& destRect);
	static void DrawImage(Gdiplus::Bitmap* bitmap, Gdiplus::Graphics* gs, RECT& destRect, RECT& corner);
};

