#include "Vector2.h"
#include <cmath>

namespace Math
{
	Vector2::Vector2()
		: _x(0.0f), _y(0.0f) {
	}

	Vector2::Vector2(float x, float y)
		: _x(x), _y(y) {
	}

	void Vector2::Set(const Vector2& vector)
	{
		_x = vector.GetX();
		_y = vector.GetY();
	}

	void Vector2::Set(float x, float y)
	{
		_x = x;
		_y = y;
	}

	void Vector2::Add(const Vector2& vector)
	{
		_x += vector.GetX();
		_y += vector.GetY();
	}

	void Vector2::Add(float x, float y)
	{
		_x += x;
		_y += y;
	}

	float Vector2::Length() const
	{
		// •½•ûª‚ğŒvZ
		return std::sqrt(_x * _x + _y * _y);
	}

	void Vector2::Normalized()
	{
		float len = Length();

		// 0œZ‚ğ–h~
		if (len > 0.0f)
		{
			_x = _x / len;
			_y = _y / len;
		}
	}

	Vector2 Vector2::Normalize() const
	{
		float len = Length();

		// 0œZ‚ğ–h~
		if (len > 0.0f)
		{
			return Vector2(_x / len, _y / len);
		}

		// ’·‚³‚ª0‚Ìê‡‚ÍA‚»‚Ì‚Ü‚Ü•Ô‚·
		return Vector2(_x, _y);
	}

	float Vector2::Dot(const Vector2& rhs) const
	{
		return _x * rhs.GetX() + _y * rhs.GetY();
	}

	float Vector2::Cross(const Vector2& rhs) const
	{
		// 2D‚É‚¨‚¯‚éŠOÏ‚ÍAƒXƒJƒ‰[’l‚Æ‚È‚é
		return _x * rhs.GetY() - _y * rhs.GetX();
	}

	Vector2 Vector2::operator+(const Vector2& rhs) const
	{
		return Vector2(_x + rhs.GetX(), _y + rhs.GetY());
	}

	Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return Vector2(_x - rhs.GetX(), _y - rhs.GetY());
	}

	Vector2 Vector2::operator*(float rhs) const
	{
		return Vector2(_x * rhs, _y * rhs);
	}

	Vector2 Vector2::operator/(float rhs) const
	{
		return Vector2(_x / rhs, _y / rhs);
	}
}