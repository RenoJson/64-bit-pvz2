#pragma once
#include <cstdint>
#include <string>
#include <stddef.h>
typedef std::string SexyString;
typedef SexyString::iterator SexyChar;
typedef float pvztime_t;

typedef uint8_t byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef signed long long int64;
typedef unsigned long long uint64;

namespace Sexy
{
	struct SexyVector2
	{
		float x, y;

		SexyVector2() : x(0), y(0) {};
		SexyVector2(float theX, float theY) : x(theX), y(theY) {};
		~SexyVector2() {}
	};

	struct SexyVector3
	{
		float x, y, z;

		SexyVector3() : x(0), y(0), z(0) {};
		SexyVector3(float theX, float theY, float theZ) : x(theX), y(theY), z(theZ) {};
		~SexyVector3() {}
	};

	struct Rect
	{
		int mX, mY, mWidth, mHeight;

		Rect() : mX(0), mY(0), mWidth(0), mHeight(0) {};
		Rect(int theX, int theY, int theWidth, int theHeight) : mX(theX), mY(theY), mWidth(theWidth), mHeight(theHeight) {};
		~Rect() {}
	};

	struct FRect
	{
		float mX, mY, mWidth, mHeight;

		FRect() : mX(0), mY(0), mWidth(0), mHeight(0) {};
		FRect(float theX, float theY, float theWidth, float theHeight) : mX(theX), mY(theY), mWidth(theWidth), mHeight(theHeight) {};
		~FRect() {}
	};

	struct Point
	{
		int mX, mY;

		Point() : mX(0), mY(0) {};
		Point(int theX, int theY) : mX(theX), mY(theY) {};
	};

	struct FPoint
	{
		float mX, mY;

		FPoint() : mX(0), mY(0) {};
		FPoint(float theX, float theY) : mX(theX), mY(theY) {};
	};

	struct ValueRange
	{
		float Min, Max;

		ValueRange() : Min(0), Max(0) {};
		ValueRange(float theMin, float theMax) : Min(theMin), Max(theMax) {};
	};

	class Color
	{
	public:

		int mRed, mGreen, mBlue, mAlpha;

		Color() : mRed(0), mGreen(0), mBlue(0), mAlpha(0) {};
		Color(int theRed, int theGreen, int theBlue, int theAlpha) : mRed(theRed), mGreen(theGreen), mBlue(theBlue), mAlpha(theAlpha) {};
	};
}