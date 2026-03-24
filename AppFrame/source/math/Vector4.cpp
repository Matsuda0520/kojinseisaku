#include "Vector4.h"
#include <cmath>

namespace Math
{
	Vector4::Vector4()
		: _x(0.0f), _y(0.0f), _z(0.0f), _w(1.0f) {
	}

	Vector4::Vector4(float x, float y, float z)
		: _x(x), _y(y), _z(z), _w(1.0f) {
	}

	Vector4::Vector4(float x, float y, float z, float w)
		: _x(x), _y(y), _z(z), _w(w) {
	}

	void Vector4::Set(const Vector4& vector)
	{
		_x = vector.GetX();
		_y = vector.GetY();
		_z = vector.GetZ();
		_w = vector.GetW();
	}

	void Vector4::Set(float x, float y, float z)
	{
		_x = x;
		_y = y;
		_z = z;
	}

	void Vector4::Add(const Vector4& vector)
	{
		_x += vector.GetX();
		_y += vector.GetY();
		_z += vector.GetZ();
	}

	void Vector4::Add(float x, float y, float z)
	{
		_x += x;
		_y += y;
		_z += z;
	}

	float Vector4::Length() const
	{
		// •½•ûª‚ğŒvZ
		return std::sqrt(_x * _x + _y * _y + _z * _z);
	}

	void Vector4::Normalized()
	{
		float len = Length();

		// 0œZ‚ğ–h~
		if (len > 0.0f)
		{
			_x = _x / len;
			_y = _y / len;
			_z = _z / len;
		}
	}

	Vector4 Vector4::Normalize() const
	{
		float len = Length();

		// 0œZ‚ğ–h~
		if (len > 0.0f)
		{
			float nx = _x / len;
			float ny = _y / len;
			float nz = _z / len;

			return Vector4(nx, ny, nz);
		}

		// ’·‚³‚ª0‚Ìê‡‚ÍA‚»‚Ì‚Ü‚Ü•Ô‚·
		return Vector4(_x, _y, _z);
	}

	float Vector4::Dot(const Vector4& rhs) const
	{
		return _x * rhs.GetX() + _y * rhs.GetY() + _z * rhs.GetZ();
	}

	Vector4 Vector4::Cross(const Vector4& rhs) const
	{
		float cx = _y * rhs.GetZ() - _z * rhs.GetY();
		float cy = _z * rhs.GetX() - _x * rhs.GetZ();
		float cz = _x * rhs.GetY() - _y * rhs.GetX();

		// W¬•ª‚Í0‚É‚·‚é
		return Vector4(cx, cy, cz, 0.0f);
	}

	Vector4 Vector4::operator+(const Vector4& rhs) const
	{
		return Vector4(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
	}

	Vector4 Vector4::operator-(const Vector4& rhs) const
	{
		return Vector4(_x - rhs.GetX(), _y - rhs.GetY(), _z - rhs.GetZ());
	}

	Vector4 Vector4::operator*(float rhs) const
	{
		return Vector4(_x * rhs, _y * rhs, _z * rhs);
	}

	Vector4 Vector4::operator/(float rhs) const
	{
		return Vector4(_x / rhs, _y / rhs, _z / rhs);
	}
}
