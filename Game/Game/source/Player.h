#pragma once
#include "CharacterBase.h"
#include "ICollider.h"
#include "IAttacker.h"
#include "AnimManager.h"
#include "CapsuleCollider.h"

class IPlayerObserver;
class PlayerState;

class Player : public CharacterBase
{
public:
	Player(const char* name = "Player");
	virtual ~Player() override = default;

	void Initialize() override;
	void Process() override;
	void Terminate() override;

	void Move() override;
	void TakeDamage(float damage) override;

	// ステートの切り替え
	void ChangeState(std::unique_ptr<PlayerState> newState);

	// 入力情報から移動ベクトルを計算して返す
	Vector4 GetInputVelocity() const;

	// アニメーション再生要求
	bool PlayAnimation(const char* animName, float blendFrame, int loopCnt, float playSpeed = 1.0f);

	// Observer管理
	//void AddObserver(IPlayerObserver* observer);
	//void RemoveObserver(IPlayerObserver* observer);

	// 自分が保持しているコライダーを返す
	ICollider* AsCollider() override { return _collider; }
	const ICollider* AsCollider() const override { return _collider; }

	// 衝突イベント
	void OnCollision(GameObject* other) override;

protected:
	//void NotifyDamage();
	//void NotifyDeath();

private:
	std::vector<IPlayerObserver*> _observers;
	std::unique_ptr<PlayerState> _currentState;// 現在の状態
	AnimManager _animManager;
	CapsuleCollider* _collider;// コライダーの参照
};

