
#include "UpImageUtility.h"
#include <GdiPlus.h>
#include "UpFile.h"
#include <atlimage.h>

using namespace Gdiplus;

const  wchar_t* UpImageUtility::FILE_TYPE_JPG = L"image/jpeg";
const  wchar_t* UpImageUtility::FILE_TYPE_BMP = L"image/bmp";
const  wchar_t* UpImageUtility::FILE_TYPE_PNG = L"image/png";

void UpImageUtility::Draw9Grid(Bitmap* bitmap, Graphics* gs, RECT& destRect)
{
	Rect rc(destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top);
	Rect srcRect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	int destWidth = (destRect.right - destRect.left) / 3;
	int destHeight = (destRect.bottom - destRect.top) / 3;
	int srcWidth =  srcRect.Width / 3;
    int srcHeight = srcRect.Height / 3;

	// left-top
	rc.Width =  destWidth;
	rc.Height = destHeight;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft(), srcRect.GetTop(), srcWidth, srcHeight, UnitPixel);
		
	// top
	rc.X += destWidth;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + srcWidth, srcRect.GetTop(), srcWidth, srcHeight, UnitPixel);

	//right-top
	rc.X += destWidth;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + 2 * srcWidth, srcRect.GetTop(), srcWidth, srcHeight, UnitPixel);


	//left-middle
	rc.X = destRect.left;
	rc.Y += destHeight;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft(), srcRect.GetTop() + srcHeight, srcWidth, srcHeight, UnitPixel);

	//middle
	rc.X += destWidth;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + srcWidth, srcRect.GetTop() + srcHeight, srcWidth, srcHeight, UnitPixel);

	//right-middle
	rc.X += destWidth;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + 2* srcWidth, srcRect.GetTop() + srcHeight, srcWidth, srcHeight, UnitPixel);

	//left-bottom
	rc.X = destRect.left;
	rc.Y+= destHeight;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft(), srcRect.GetTop() +  2* srcHeight, srcWidth, srcHeight, UnitPixel);

	//bottom
	rc.X += destWidth;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + srcWidth, srcRect.GetTop() +  2* srcHeight, srcWidth, srcHeight, UnitPixel);

	//right-bottom
	rc.X += destWidth;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + 2 * srcWidth, srcRect.GetTop() +  2* srcHeight, srcWidth, srcHeight, UnitPixel);
}

void UpImageUtility::DrawImage(Bitmap* bitmap, Graphics* gs, RECT& destRect, RECT& corner)
{
	Rect rc(destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top);
	Rect srcRect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());

	// left-top
	rc.Width =  corner.left;
	rc.Height = corner.top;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft(), srcRect.GetTop(),  corner.left, corner.top, UnitPixel);

	// top
	rc.X += corner.left;
	rc.Width = destRect.right - destRect.left - corner.right -corner.left;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + corner.left,  srcRect.GetTop(), srcRect.Width - corner.right -corner.left, corner.top, UnitPixel);

	//right-top
	rc.X = destRect.right - corner.right;
	rc.Width = corner.right;
	gs->DrawImage(bitmap, rc, srcRect.GetRight() - corner.right , srcRect.GetTop(), corner.right, corner.top, UnitPixel);


	//left-middle
	rc.X = destRect.left;
	rc.Y += corner.top;
	rc.Width = corner.left;
	rc.Height = destRect.bottom - destRect.top - corner.bottom - corner.top;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft(), srcRect.GetTop() + corner.top, corner.left , srcRect.Height - corner.bottom - corner.top, UnitPixel);

	//middle
	rc.X += corner.left;
    rc.Width = destRect.right - destRect.left - corner.right -corner.left;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + corner.left, srcRect.GetTop() + corner.top, srcRect.Width - corner.right -corner.left, srcRect.Height - corner.bottom - corner.top, UnitPixel);

	//right-middle
	rc.X =  destRect.right - corner.right;
	rc.Width = corner.right;
	gs->DrawImage(bitmap, rc, srcRect.GetRight() - corner.right, srcRect.GetTop() + corner.top, corner.right, srcRect.Height - corner.bottom - corner.top, UnitPixel);

	//left-bottom
	rc.X = destRect.left;
	rc.Y= destRect.bottom - corner.bottom;
	rc.Width = corner.left;
	rc.Height = corner.bottom;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft(), srcRect.GetBottom() - corner.bottom, corner.left, corner.bottom, UnitPixel);

	//bottom
	rc.X += corner.left;
	rc.Width = destRect.right - destRect.left - corner.right -corner.left;
	gs->DrawImage(bitmap, rc, srcRect.GetLeft() + corner.left, srcRect.GetBottom() - corner.bottom, srcRect.Width - corner.right -corner.left, corner.bottom, UnitPixel);
	//right-bottom
	rc.X = destRect.right - corner.right;
	rc.Width = corner.right;
	gs->DrawImage(bitmap, rc, srcRect.GetRight() - corner.right,srcRect.GetBottom() - corner.bottom , corner.right, corner.bottom, UnitPixel);
}