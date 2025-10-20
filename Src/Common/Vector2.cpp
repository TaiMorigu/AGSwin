#include "Vector2.h"

#include<algorithm>

// コンストラクタ
Vector2I::Vector2I(void)
{
	x = 0;
	y = 0;
}

// コンストラクタ
Vector2I::Vector2I(int vX, int vY)
{
	x = vX;
	y = vY;
}

Vector2I::~Vector2I(void)
{
}

void Vector2I::operator=(const int value)
{
	x = value; y = value;
}

Vector2I Vector2I::operator+(const Vector2I value)const
{
	return Vector2I(x + value.x, y + value.y);
}

void Vector2I::operator+=(const Vector2I value)
{
	x = x + value.x;
	y = y + value.y;
}

Vector2I Vector2I::operator+(const int value) const
{
	return Vector2I(x + value, y + value);
}

void Vector2I::operator+=(const int value)
{
	x += value; y += value;
}

Vector2I Vector2I::operator-(const Vector2I value)const
{
	return Vector2I(x - value.x, y - value.y);
}

void Vector2I::operator-=(const Vector2I value)
{
	x = x - value.x;
	y = y - value.y;
}

Vector2I Vector2I::operator-(const int value) const
{
	return Vector2I(x - value, y - value);
}

void Vector2I::operator-=(const int value)
{
	x -= value; y -= value;
}

Vector2I Vector2I::operator*(const Vector2I value) const
{
	return Vector2I(x * value.x, y * value.y);
}

void Vector2I::operator*=(const Vector2I value)
{
	x *= value.x; y *= value.y;
}

Vector2I Vector2I::operator*(const int value)const
{
	return Vector2I(x * value, y * value);
}

void Vector2I::operator*=(const int value)
{
	x = x * value;
	y = y * value;
}

Vector2I Vector2I::operator/(const Vector2I value) const
{
	return Vector2I(x / value.x, y / value.y);
}

void Vector2I::operator/=(const Vector2I value)
{
	x /= value.x; y /= value.y;
}

Vector2I Vector2I::operator/(const int value)const
{
	return Vector2I(x / value, y / value);
}

void Vector2I::operator/=(const int value)
{
	x = x / value;
	y = y / value;
}

bool Vector2I::operator==(const int value) const
{
	return (x == value && y == value);
}

bool Vector2I::operator!=(const int value) const
{
	return (x != value || y != value);
}

Vector2 Vector2I::ToFloat(void) const
{
	return Vector2(static_cast<float>(x), static_cast<float>(y));
}



Vector2::Vector2(void)
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float vX, float vY)
{
	x = vX;
	y = vY;
}

Vector2::~Vector2(void)
{
}

void Vector2::operator=(const float value)
{
	x = value; y = value;
}

Vector2 Vector2::operator+(const Vector2 value) const
{
	return Vector2(x + value.x, y + value.y);
}

void Vector2::operator+=(const Vector2 value)
{
	x = x + value.x;
	y = y + value.y;
}

Vector2 Vector2::operator+(const float value) const
{
	return Vector2(x + value, y + value);
}

void Vector2::operator+=(const float value)
{
	x += value;
	y += value;
}

Vector2 Vector2::operator-(const Vector2 value) const
{
	return Vector2(x - value.x, y - value.y);
}

void Vector2::operator-=(const Vector2 value)
{
	x = x - value.x;
	y = y - value.y;
}

Vector2 Vector2::operator-(const float value) const
{
	return Vector2(x - value, y - value);
}

void Vector2::operator-=(const float value)
{
	x -= value;
	y -= value;
}

Vector2 Vector2::operator*(const Vector2 value) const
{
	return Vector2(x * value.x, y * value.y);
}

void Vector2::operator*=(const Vector2 value)
{
	x *= value.x;
	y *= value.y;
}

Vector2 Vector2::operator*(const float value) const
{
	return Vector2(x * value, y * value);
}

void Vector2::operator*=(const float value)
{
	x = x * value;
	y = y * value;
}

Vector2 Vector2::operator/(const Vector2 value) const
{
	return Vector2(x / value.x, y / value.y);
}

void Vector2::operator/=(const Vector2 value)
{
	x /= value.x;
	y /= value.y;
}

Vector2 Vector2::operator/(const float value) const
{
	return Vector2(x / value, y / value);
}

void Vector2::operator/=(const float value)
{
	x = x / value;
	y = y / value;
}

bool Vector2::operator==(const float value) const
{
	return (x == value && y == value);
}

bool Vector2::operator!=(const float value) const
{
	return (x != value || y != value);
}

bool Vector2::operator==(const Vector2 value) const
{
	return (x == value.x && y == value.y);
}

bool Vector2::operator!=(const Vector2 value) const
{
	return (x != value.x || y != value.y);
}

bool Vector2::operator<=(const float value) const
{
	return (x <= value && y <= value);
}

bool Vector2::operator<=(const Vector2 value) const
{
	return (x <= value.x && y <= value.y);
}

bool Vector2::operator<(const float value) const
{
	return (x < value && y < value);
}

bool Vector2::operator<(const Vector2 value) const
{
	return (x < value.x && y < value.y);
}

bool Vector2::operator>=(const float value) const
{
	return (x >= value && y >= value);
}

bool Vector2::operator>=(const Vector2 value) const
{
	return (x >= value.x && y >= value.y);
}

bool Vector2::operator>(const float value) const
{
	return (x > value && y > value);
}

bool Vector2::operator>(const Vector2 value) const
{
	return (x > value.x && y > value.y);
}

Vector2 Vector2::Abs(void) const
{
	return { abs(x),abs(y) };
}

float Vector2::length(void) const
{
	return sqrtf(x * x + y * y);
}

Vector2I Vector2::ToInt(void) const
{
	return Vector2I(static_cast<int>(x), static_cast<int>(y));
}
