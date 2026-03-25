#pragma once
#include "Laser.h"

class NormalLaser : public Laser
{
public:
	NormalLaser(const char* name = "NormalLaser")
		: Laser(name) {}

	void Setup(float centerY, float baseZ) override
	{
		_baseY = centerY;
		_baseZ = baseZ;

		// 固定レーザーのため、一度だけ設定
		Vector4 start(-400.0f, _baseY, _baseZ);
		Vector4 end(400.0f, _baseY, _baseZ);
		SetCapsule(start, end, _capsuleRadius);
	}
};
