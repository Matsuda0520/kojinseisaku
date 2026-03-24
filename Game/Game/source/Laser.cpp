#include "Laser.h"

Laser::Laser(const char* name)
	: GameComposite(name)
	, _collider(nullptr)
	, _capsuleStart()
	, _capsuleEnd(0.0f, 0.0f, 100.0f)
	, _capsuleRadius(20.0f)
	, _attackPower(10.0f)
{
}

void Laser::Initialize()
{
	GameComposite::Initialize();

	// コライダーを生成して、子として追加する
	auto collider = std::make_unique<CapsuleCollider>(
		"LaserCollider",
		CollisionLayer::Enemy,
		_capsuleRadius, 200.0f,// 半径、高さ
		this
	);
	_collider = collider.get();// アクセス用にポインタを保持
	AddChild(std::move(collider));// 所有権は親へ渡す
}

void Laser::Render()
{
	if (_isDead) { return; }

	// test
	DrawLine3D(ToDX(_capsuleStart), ToDX(_capsuleEnd), GetColor(255, 0, 0));

	GameComposite::Render();
}

void Laser::SetCapsule(const Vector4& start, const Vector4& end, float radius)
{
	_capsuleStart = start;
	_capsuleEnd = end;
	_capsuleRadius = radius;

	// カプセルの中心点を計算して座標にセット
	_pos.Set(
		(start.GetX() + end.GetX()) * 0.5f,
		(start.GetY() + end.GetY()) * 0.5f,
		(start.GetZ() + end.GetZ()) * 0.5f
	);
}
