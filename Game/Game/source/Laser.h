#pragma once
#include "GameComposite.h"
#include "IAttacker.h"
#include "CapsuleCollider.h"

// レーザーオブジェクト
class Laser : public GameComposite, public IAttacker
{
public:
	Laser(const char* name = "Laser");
	virtual ~Laser() override = default;

	void Initialize() override;
	void Process() override;
	void Render() override;

	// カプセル設定(始点、終点、半径)(ワールド座標)
	void SetCapsule(const Vector4& start, const Vector4& end, float radius);
	void SetAttackPower(float power) { _attackPower = power; }

	// プール用の状態管理関数
	void Sleep();// 非アクティブにする
	void Revive();// アクティブにする
	bool IsSleeping() const { return _isSleeping; }

	// スポナーから基準となるY座標とZ座標を受け取って、位置をセットする
	virtual void Setup(float centerY, float baseZ) = 0;

	// IAttacker
	float GetAttackPower() const override { return _attackPower; }

	// GameObject型でのアクセス用
	ICollider* AsCollider() override { return _collider; }
	const ICollider* AsCollider() const override { return _collider; }
	IAttacker* AsAttacker() override { return this; }
	const IAttacker* AsAttacker() const override { return this; }

protected:
	CapsuleCollider* _collider;// コライダーの参照

	Vector4 _capsuleStart;// カプセルの始点
	Vector4 _capsuleEnd;// カプセルの終点
	float _capsuleRadius;// カプセルの半径
	float _attackPower;// ダメージ量

	bool _isSleeping;// 非アクティブフラグ

	float _baseY;// 基準となるY座標
	float _baseZ;// 基準となるZ座標
};

