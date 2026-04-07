#pragma once
#include "appframe.h"
#include "ColliderComponent.h"

class CollisionManager
{
public:
	// シングルトンインスタンの取得
	static CollisionManager& GetInstance()
	{
		static CollisionManager instance;
		return instance;
	}

	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	void Initialize() {}
	void Terminate() {}
	void Process();
	void Render();

	void Register(ColliderComponent* collider);
	void Unregister(ColliderComponent* collider);

private:
	CollisionManager() = default;
	~CollisionManager() = default;

	// 2つのコライダーに対して当たり判定を行う
	bool CheckCollision(ColliderComponent* a, ColliderComponent* b);

	// レイヤーの組み合わせが有効なペアかチェックする
	bool CanCollide(CollisionLayer a, CollisionLayer b) const;

	// カプセルとカプセルの当たり判定
	bool CheckCapsuleCapsule(const ColliderComponent* a, const ColliderComponent* b) const;

	// カプセル同士の判定前の大まかな判定(球でチェック)
	bool CheckCapsuleCapsuleRough(const ColliderComponent* a, const ColliderComponent* b) const;

	// カプセルを内包する球を計算する
	void CalcCapsuleRoughSphere(const ColliderComponent* capsule, VECTOR& outCenter, float& outRadius) const;

	std::vector<ColliderComponent*> _colliders;

	// 前フレームの衝突を記録するセット
	std::set<std::pair<ColliderComponent*, ColliderComponent*>> _prevHits;

	static constexpr std::array s_collisonPairs =
	{
		std::pair{ CollisionLayer::Player, CollisionLayer::Enemy },
		std::pair{ CollisionLayer::Player, CollisionLayer::Item },
	};
};

