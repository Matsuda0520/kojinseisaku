#pragma once
#include "GameObject.h"
#include "ICollider.h"

// 全オブジェクトの当たり判定を管理するマネージャー
class CollisionManager : public GameObject
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

	void Initialize() override {}
	void Process() override;
	void Render() override;
	void Terminate() override;

	// コライダーの生ポインタで登録・解除を受け付ける
	void Register(ICollider* collider);
	void Unregister(ICollider* collider);

private:
	CollisionManager(const char* name = "CollisionManager");
	~CollisionManager() override = default;

	// 2つのコライダーに対して当たり判定を行う
	bool CheckCollision(ICollider* a, ICollider* b);

	// レイヤーの組み合わせが有効なペアかチェックする
	bool CanCollide(CollisionLayer a, CollisionLayer b) const;

	// 球と球の当たり判定
	bool CheckSphereSphere(const ISphereCollider* a, const ISphereCollider* b) const;

	// カプセルとカプセルの当たり判定
	bool CheckCapsuleCapsule(const ICapsuleCollider* a, const ICapsuleCollider* b) const;

	// カプセル同士の判定前の大まかな判定(球でチェック)
	bool CheckCapsuleCapsuleRough(const ICapsuleCollider* a, const ICapsuleCollider* b) const;

	// カプセルを内包する球を計算する
	void CalcCapsuleRoughSphere(const ICapsuleCollider* capsule, VECTOR& outCenter, float& outRadius) const;

	std::vector<ICollider*> _colliders;// コライダーの生ポインタを保持するリスト

	// 前フレームの衝突を記録するセット
	std::set<std::pair<ICollider*, ICollider*>> _prevHits;

	static constexpr std::array s_collisonPairs =
	{
		std::pair{ CollisionLayer::Player, CollisionLayer::Enemy },
		std::pair{ CollisionLayer::Player, CollisionLayer::Item },
	};
};

