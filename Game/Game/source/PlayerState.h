#pragma once

class Player;

// 状態の基底クラス
class PlayerState
{
public:
	virtual ~PlayerState() = default;

	// このステートに入ったときに呼ばれる
	virtual void Enter(Player* player) {}

	// 毎フレーム呼ばれる
	virtual void Update(Player* player) = 0;

	// このステートから出るときに呼ばれる
	virtual void Exit(Player* player) {}
};

// 待機状態
class PlayerStateIdle : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
};

// 移動状態
class PlayerStateMove : public PlayerState
{
	public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
};

// ジャンプ状態
class PlayerStateJump : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Exit(Player* player) override;

private:
	float _verticalVelocity = 0.0f;// 縦方向の速度
};

// ローリング状態
class PlayerStateRoll : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Exit(Player* player) override;

private:
	float _timer = 0.0f;// ローリングタイマー
};

