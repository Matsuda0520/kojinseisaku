#include "Laser.h"
#include "CollisionManager.h"

Laser::Laser(const char* name)
	: GameComposite(name)
	, _collider(nullptr)
	, _capsuleStart()
	, _capsuleEnd(0.0f, 0.0f, 100.0f)
	, _capsuleRadius(40.0f)
	, _attackPower(10.0f)
	, _isSleeping(false)
	, _baseY(0.0f)
	, _baseZ(0.0f)
{
}

void Laser::Initialize()
{
	GameComposite::Initialize();

	// コライダーを生成して、子として追加する
	auto collider = std::make_unique<CapsuleCollider>(
		"LaserCollider",
		CollisionLayer::Enemy,
		_capsuleRadius,// 半径
		this
	);
	_collider = collider.get();// アクセス用にポインタを保持
	AddChild(std::move(collider));// 所有権は親へ渡す

	// カプセルの初期設定
	if (_collider)
	{
		_collider->SetCapsuleRadius(_capsuleRadius);
		_collider->SetCapsuleSegment(_capsuleStart, _capsuleEnd);
	}
}

void Laser::Process()
{
	// 死亡状態、非アクティブなら処理しない
	if (_isDead || _isSleeping) { return; }

	GameComposite::Process();
}

void Laser::Render()
{
	// 死亡状態、非アクティブなら描画しない
	if (_isDead || _isSleeping) { return; }

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

	// コライダー情報も更新する
	if (_collider)
	{
		_collider->SetCapsuleRadius(_capsuleRadius);
		_collider->SetCapsuleSegment(_capsuleStart, _capsuleEnd);
	}
}

void Laser::Sleep()
{
	if (_isSleeping) { return; }
	_isSleeping = true;

	// 非アクティブなら当たり判定マネージャーからも削除する
	if (_collider)
	{
		CollisionManager::GetInstance().Unregister(_collider);
	}
}

void Laser::Revive()
{
	if (!_isSleeping) { return; }
	_isSleeping = false;

	// アクティブなら当たり判定マネージャーに再登録する
	if (_collider)
	{
		CollisionManager::GetInstance().Register(_collider);
	}
}
