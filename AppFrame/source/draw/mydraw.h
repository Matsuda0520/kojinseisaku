#pragma once
#include "DxLib.h"
#include "../math/mymath.h"

// 描画ユーティリティ
namespace mydraw
{
	// DrawLine3Dを組み合わせて3D空間に円を描画する関数
	// center:中心座標, radius:半径, color:色, segment:分割数
	inline void DrawCircle3D(const VECTOR& center, float radius, unsigned int color, int segments)
	{
		float step = DX_TWO_PI_F / segments;

		VECTOR prev = VGet(
			center.x + sinf(0.0f) * radius,
			center.y,
			center.z + cosf(0.0f) * radius
		);

		for (int i = 1; i <= segments; ++i)
		{
			float angle = step * i;
			VECTOR next = VGet(
				center.x + sinf(angle) * radius,
				center.y,
				center.z + cosf(angle) * radius
			);
			DrawLine3D(prev, next, color);
			prev = next;
		}
	}

	// DrawLine3Dを組み合わせて3D空間に扇形を描画する関数
	// center:中心座標, dir:基準の向きベクトル, radius:半径, halfAngleDeg:半角(度), color:色, segments:分割数
	inline void DrawFan3D(const VECTOR& center, const VECTOR& dir, float radius, float halfAngleDeg, unsigned int color, int segments)
	{
		float halfAngleRad = halfAngleDeg * Math::DEGREE_TO_RADIAN;
		float currentDirAngle = atan2f(dir.x, dir.z);
		float startAngle = currentDirAngle - halfAngleRad;
		float totalAngle = halfAngleRad * 2.0f;

		VECTOR leftEdge = VAdd(center, VGet(sinf(startAngle) * radius, 0.0f, cosf(startAngle) * radius));
		float  endAngle = startAngle + totalAngle;
		VECTOR rightEdge = VAdd(center, VGet(sinf(endAngle) * radius, 0.0f, cosf(endAngle) * radius));

		DrawLine3D(center, leftEdge, color);
		DrawLine3D(center, rightEdge, color);

		VECTOR vPrev = leftEdge;
		for (int i = 1; i <= segments; ++i)
		{
			float ratio = static_cast<float>(i) / static_cast<float>(segments);
			float angle = startAngle + totalAngle * ratio;
			VECTOR next = VAdd(center, VGet(sinf(angle) * radius, 0.0f, cosf(angle) * radius));
			DrawLine3D(vPrev, next, color);
			vPrev = next;
		}
	}
}
