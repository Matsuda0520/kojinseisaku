#pragma once
#include "DxLib.h"
#include "../math/Vector4.h"
#include "../math/Vector2.h"

VECTOR ToDX(const Math::Vector4& v);
Math::Vector4 ToMath(const VECTOR& v);

VECTOR ToDX(const Math::Vector2& v);

