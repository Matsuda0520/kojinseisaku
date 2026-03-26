#pragma once
#include "CharacterBase.h"
#include "ICollider.h"
#include "IAttacker.h"
#include "AnimManager.h"
#include "CapsuleCollider.h"

class IHPObserver;
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

	// ローリング中のカプセル設定
	void SetRollingCollider();
	void ResetCollider();

	// 自分が保持しているコライダーを返す
	ICollider* AsCollider() override { return _collider; }
	const ICollider* AsCollider() const override { return _collider; }
	virtual Player* AsPlayer() override { return this; }

	// 状態別の衝突イベント
	void OnCollisionEnter(GameObject* other) override;
	void OnCollisionStay(GameObject* other) override {}
	void OnCollisionExit(GameObject* other) override {}

	float GetSpeedMultiplier() const { return _speedMultiplier; }
	float GetSpeed() const override { return _speed * _speedMultiplier; }

	void SetSpeedUpActive(bool active) { _isSpeedUpActive = active; }

private:
	// カプセルのパラメータを適用して、コライダーの線分を更新する
	void ApplyCapsule(float radius, float halfHeight);
	// プレイヤーの位置からカプセルの線分を更新する
	void UpdateCapsuleSegment();

	std::unique_ptr<PlayerState> _currentState;// 現在の状態
	AnimManager _animManager;
	CapsuleCollider* _collider;// コライダーの参照

	float _capsuleRadius;// カプセルの半径
	float _capsuleHalfHeight;// カプセルの半分の高さ

	float _speedMultiplier = 1.0f;// 移動速度の倍率
	bool _isSpeedUpActive = false;
};

