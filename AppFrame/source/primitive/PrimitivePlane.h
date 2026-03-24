#pragma once
#include "PrimitiveBase.h"
#include "../math/Vector4.h"

using Math::Vector4;

class PrimitivePlane : public PrimitiveBase
{
public:
	PrimitivePlane() = default;
	virtual ~PrimitivePlane() = default;

	// テクスチャハンドル、原点座標、タイルサイズ、分割数を指定して平面を生成
	void CreatePlane(int grHandle, const Vector4& originPos, float tileSize, int tileX, int tileZ);

};

