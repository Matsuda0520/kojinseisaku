#pragma once
#include "Component.h"
#include "appframe.h"

// 当たり判定のグループ
enum class CollisionLayer
{
	Player,
	Enemy,
	Item,
	Max
};

// 当たり判定の形状
enum class CollisionShape
{
	Sphere,
	Capsule
};

class ColliderComponent : public Component
{
public:
	ColliderComponent(CollisionLayer layer, CollisionShape shape)
		: _layer(layer), _shape(shape) {}
	virtual ~ColliderComponent() = default;

	CollisionLayer GetLayer() const { return _layer; }
	CollisionShape GetShape() const { return _shape; }

	virtual void OnCollisionEnter(GameObject* other) {}
	virtual void OnCollisionStay(GameObject* other) {}
	virtual void OnCollisionExit(GameObject* other) {}

private:
	CollisionLayer _layer;
	CollisionShape _shape;

};

