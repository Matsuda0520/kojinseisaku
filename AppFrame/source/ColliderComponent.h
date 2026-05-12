#pragma once
#include "Component.h"

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

// ゲームオブジェクトに当たり判定を持たせるためのコンポーネント
// 派生先のクラスで、形状ごとの当たり判定情報と衝突時の処理を実装する
class ColliderComponent : public Component
{
public:
	// コンストラクタで当たり判定のレイヤーと形状を指定する
	ColliderComponent(CollisionLayer layer, CollisionShape shape)
		: _layer(layer), _shape(shape) {}
	virtual ~ColliderComponent() = default;

	CollisionLayer GetLayer() const { return _layer; }
	CollisionShape GetShape() const { return _shape; }

	virtual void OnCollisionEnter(GameObject* other) {}// 衝突開始時の処理。衝突した相手のGameObjectポインタを受け取る
	virtual void OnCollisionStay(GameObject* other) {}// 衝突中の処理。衝突した相手のGameObjectポインタを受け取る
	virtual void OnCollisionExit(GameObject* other) {}// 衝突終了時の処理。衝突した相手のGameObjectポインタを受け取る

private:
	CollisionLayer _layer;
	CollisionShape _shape;
};

