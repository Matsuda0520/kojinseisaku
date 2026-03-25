#pragma once
#include "Laser.h"

// 中心を軸にしてXY平面上で回転するレーザー
class RotatingLaser : public Laser
{
public:
	RotatingLaser(const char* name = "RotatingLaser");

	void Process() override;

	void Setup(float centerY, float baseZ) override;

private:
	void UpdateCapsule();// カプセル座標を更新する

	float _angle;
	float _rotateSpeed;// 回転速度
	float _length;// 中心からの長さ
};

