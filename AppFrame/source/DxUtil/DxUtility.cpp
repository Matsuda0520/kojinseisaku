#include "DxUtility.h"

// Vector4‚ðDxLib‚ÌVECTOR‚É•ÏŠ·
VECTOR ToDX(const Math::Vector4& v)
{
	return VGet(v.GetX(), v.GetY(), v.GetZ());
}

// DxLib‚ÌVECTOR‚ðVector4‚É•ÏŠ·
Math::Vector4 ToMath(const VECTOR& v)
{
	return Math::Vector4(v.x, v.y, v.z, 1.0f);
}

// Vector2‚ðDxLib‚ÌVECTOR‚É•ÏŠ·
VECTOR ToDX(const Math::Vector2& v)
{
	return VGet(v.GetX(), v.GetY(), 0.0f);
}
