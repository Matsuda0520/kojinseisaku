#pragma once
#include "GameComposite.h"
#include "IAttacker.h"
#include "CapsuleCollider.h"

// レーザーオブジェクト
class Laser : public GameComposite, public IAttacker
{
public:
	Laser(const char* name = "Laser");
	~Laser() override = default;

	void Initialize() override;
	void Render() override;

	// カプセル設定(始点、終点、半径)(ワールド座標)
	void SetCapsule(const Vector4& start, const Vector4& end, float radius);
	void SetAttackPower(float power) { _attackPower = power; }

	// IAttacker
	float GetAttackPower() const override { return _attackPower; }

	// GameObject型でのアクセス用
	ICollider* AsCollider() override { return _collider; }
	const ICollider* AsCollider() const override { return _collider; }
	IAttacker* AsAttacker() override { return this; }
	const IAttacker* AsAttacker() const override { return this; }

private:
	CapsuleCollider* _collider;// コライダーの参照

	Vector4 _capsuleStart;// カプセルの始点
	Vector4 _capsuleEnd;// カプセルの終点
	float _capsuleRadius;// カプセルの半径
	float _attackPower;// ダメージ量
};

