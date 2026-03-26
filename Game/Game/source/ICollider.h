#pragma once
#include "appframe.h"

class GameObject;

// 当たり判定のグループを定義
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

// 球コライダー用インターフェース
class ISphereCollider
{
public:
	virtual ~ISphereCollider() = default;

	// 球の中心座標(ワールド座標)
	virtual Vector4 GetSphereCenter() const = 0;

	// 球の半径
	virtual float GetSphereRadius() const = 0;
};

// カプセルコライダー用インターフェース
class ICapsuleCollider
{
public:
	virtual ~ICapsuleCollider() = default;

	// カプセルの始点(ワールド座標)
	virtual Vector4 GetCapsuleStart() const = 0;

	// カプセルの終点(ワールド座標)
	virtual Vector4 GetCapsuleEnd() const = 0;

	// カプセル半径
	virtual float GetCapsuleRadius() const = 0;
};

// 当たり判定の共通インターフェース
class ICollider
{
public:
	virtual ~ICollider() = default;

	// 自身の所属レイヤー
	virtual CollisionLayer GetLayer() const = 0;

	// 形状種別
	virtual CollisionShape GetShapeType() const = 0;

	// 形状別インターフェース取得
	virtual ISphereCollider* AsSphereCollider() { return nullptr; }
	virtual const ISphereCollider* AsSphereCollider() const { return nullptr; }

	virtual ICapsuleCollider* AsCapsuleCollider() { return nullptr; }
	virtual const ICapsuleCollider* AsCapsuleCollider() const { return nullptr; }

	// 当たった瞬間に呼ばれる
	virtual void OnCollisionEnter(GameObject* other) {}
	// 当たっている間呼ばれる
	virtual void OnCollisionStay(GameObject* other) {}
	// 離れた瞬間に呼ばれる
	virtual void OnCollisionExit(GameObject* other) {}

	// 自身をGameObjectとして返す
	virtual GameObject* GetOwner() = 0;
};
