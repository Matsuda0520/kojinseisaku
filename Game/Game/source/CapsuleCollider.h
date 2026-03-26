#pragma once
#include "GameLeaf.h"
#include "ICollider.h"

class CollisionManager;

class CapsuleCollider : public GameLeaf, public ICollider, public ICapsuleCollider
{
public:
	CapsuleCollider(const char* name, CollisionLayer layer, float radius, GameObject* owner);
	virtual ~CapsuleCollider();

	void Initialize() override;

	// インターフェース実装
	ICollider* AsCollider() override { return this; }
	const ICollider* AsCollider() const override { return this; }

	CollisionLayer GetLayer() const override { return _layer; }
	CollisionShape GetShapeType() const override { return CollisionShape::Capsule; }
	ICapsuleCollider* AsCapsuleCollider() override { return this; }
	const ICapsuleCollider* AsCapsuleCollider() const override { return this; }

	// オーナーを返す
	GameObject* GetOwner() override { return _owner; }

	// 衝突時はオーナーの対応するイベントを呼び出す
	void OnCollisionEnter(GameObject* other) override;
	void OnCollisionStay(GameObject* other) override;
	void OnCollisionExit(GameObject* other) override;

	// カプセル情報(線分 + 半径)で定義
	Vector4 GetCapsuleStart() const override { return _segmentStart; }
	Vector4 GetCapsuleEnd() const override { return _segmentEnd; }
	float GetCapsuleRadius() const override { return _radius; }

	void SetCapsuleRadius(float radius) { _radius = radius; }
	void SetCapsuleSegment(const Vector4& start, const Vector4& end);

private:
	CollisionLayer _layer;
	float _radius;
	GameObject* _owner;

	Vector4 _segmentStart;// 線分の始点
	Vector4 _segmentEnd;// 線分の終点

};

