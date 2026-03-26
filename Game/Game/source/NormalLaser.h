#pragma once
#include "Laser.h"

class NormalLaser : public Laser
{
public:
	NormalLaser(const char* name = "NormalLaser")
		: Laser(name) {}

	void Process() override
	{
		if (_isDead || _isSleeping) { return; }

		UpdateCapsule();

		Laser::Process();
	}

	void Setup(float centerY, float baseZ) override
	{
		_baseY = centerY;
		_baseZ = baseZ;

		UpdateCapsule();
	}

private:
	void UpdateCapsule()
	{
		Vector4 start(-400.0f, _baseY, _baseZ);
		Vector4 end(400.0f, _baseY, _baseZ);
		SetCapsule(start, end, _capsuleRadius);
	}
};
