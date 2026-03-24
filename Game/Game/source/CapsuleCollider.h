#pragma once
#include "GameLeaf.h"
#include "ICollider.h"

class CollisionManager;

class CapsuleCollider : public GameLeaf, public ICollider, public ICapsuleCollider
{
public:
	CapsuleCollider(const char* name, CollisionLayer layer, float radius, float halfHeight, GameObject* owner);
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

	// 衝突時はオーナーのOnCollisionを呼び出す
	void OnCollision(GameObject* other) override;

	// オーナーの座標に追従する
	Vector4 GetCapsuleStart() const override;
	Vector4 GetCapsuleEnd() const override;
	float GetCapsuleRadius() const override { return _radius; }

private:
	CollisionLayer _layer;
	float _radius;
	float _halfHeight;
	GameObject* _owner;

};

