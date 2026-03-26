#include "RotatingLaser.h"

RotatingLaser::RotatingLaser(const char* name)
	: Laser(name)
	, _angle(0.0f)
	, _rotateSpeed(0.015f)
	, _length(400.0f)
{
}

void RotatingLaser::Process()
{
	if (_isDead || _isSleeping) { return; }

	_angle += _rotateSpeed;// 回転角度を更新
	UpdateCapsule();// 毎フレームカプセル座標を更新

	Laser::Process();
}

void RotatingLaser::Setup(float centerY, float baseZ)
{
	// スポナーからの基準位置を保存
	_baseY = 180.0f;// Y座標は固定
	_baseZ = baseZ;
	_angle = 0.0f;// 同じ動き始めにするため、角度をリセット
	UpdateCapsule();
}

void RotatingLaser::UpdateCapsule()
{
	float cosA = std::cos(_angle);
	float sinA = std::sin(_angle);

	// 中心(θ, baseY, baseZ)から、両端の座標を計算
	Vector4 start(-_length * cosA, _baseY - _length * sinA, _baseZ);
	Vector4 end(_length * cosA, _baseY + _length * sinA, _baseZ);
	SetCapsule(start, end, _capsuleRadius);
}
