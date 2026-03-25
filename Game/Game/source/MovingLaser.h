#pragma once
#include "Laser.h"

// sin波で上下に動くレーザー
class MovingLaser : public Laser
{
public:
	MovingLaser(const char* name = "MovingLaser");

	void Process() override;

	void Setup(float centerY, float baseZ) override;

private:
	void UpdateCapsule();// カプセル座標を更新する

	float _time;
	float _amplitude;// 上下の振れ幅
	float _speed;// 動く速さ
};

