#include "MovingLaser.h"

MovingLaser::MovingLaser(const char* name)
	: Laser(name)
	, _time(0.0f)
	, _amplitude(150.0f)
	, _speed(0.03f)
{
}

void MovingLaser::Process()
{
	if (_isDead || _isSleeping) { return; }

	_time += 1.0f;// 時間を更新
	UpdateCapsule();// 毎フレームカプセル座標を更新

	Laser::Process();
}

void MovingLaser::Setup(float centerY, float baseZ)
{
	// スポナーからの基準位置を保存
	_baseY = 150.0f;// Y座標は固定
	_baseZ = baseZ;
	_time = 0.0f;// 同じ動き始めにするため、時間をリセット
	UpdateCapsule();
}

void MovingLaser::UpdateCapsule()
{
	// sin波で上下させる
	// _baseY:上下移動の中心、sin(_time * _speed):-1.0～1.0の周期、_amplitude:振れ幅
	float currentY = _baseY + std::sin(_time * _speed) * _amplitude;

	Vector4 start(-400.0f, currentY, _baseZ);
	Vector4 end(400.0f, currentY, _baseZ);
	SetCapsule(start, end, _capsuleRadius);
}
