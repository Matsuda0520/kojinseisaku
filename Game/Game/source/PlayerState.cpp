#include "PlayerState.h"
#include "Player.h"
#include "ActionMap.h"

// 待機状態
void PlayerStateIdle::Enter(Player* player)
{
	// 待機状態に入るときの処理
	player->PlayAnimation("mainRig|Run", 10.0f, 0, 0.5f);
}

void PlayerStateIdle::Update(Player* player)
{
	auto& actionMap = ActionMap::GetInstance();

	// ジャンプ入力のチェック
	if(actionMap.IsTrigger(InputAction::Jump))
	{
		player->ChangeState(std::make_unique<PlayerStateJump>());
		return;
	}

	// ローリング入力のチェック
	if (actionMap.IsTrigger(InputAction::Roll))
	{
		player->ChangeState(std::make_unique<PlayerStateRoll>());
		return;
	}

	// 横移動入力のチェック
	Vector4 inputVel = player->GetInputVelocity();
	if (std::abs(inputVel.GetX()) > 0.0f)
	{
		player->ChangeState(std::make_unique<PlayerStateMove>());
		return;
	}

	// 常に前進
	Vector4 pos = player->GetPosition();
	pos = pos + Vector4(0.0f, 0.0f, player->GetSpeed());
	player->SetPosition(pos);
}	

// 移動状態
void PlayerStateMove::Enter(Player* player)
{
	// 移動状態に入るときの処理
}

void PlayerStateMove::Update(Player* player)
{
	auto& actionMap = ActionMap::GetInstance();

	// ジャンプ入力のチェック
	if(actionMap.IsTrigger(InputAction::Jump))
	{
		player->ChangeState(std::make_unique<PlayerStateJump>());
		return;
	}

	// ローリング入力のチェック
	if (actionMap.IsTrigger(InputAction::Roll))
	{
		player->ChangeState(std::make_unique<PlayerStateRoll>());
		return;
	}

	// 横入力がなくなったら直進状態に戻る
	Vector4 inputVel = player->GetInputVelocity();
	if (std::abs(inputVel.GetX()) == 0.0f)
	{
		player->ChangeState(std::make_unique<PlayerStateIdle>());
		return;
	}

	// 座標の更新
	Vector4 pos = player->GetPosition();
	// X方向には入力値 * スピード、Z方向には常にスピードを加算
	pos = pos + Vector4(inputVel.GetX() * player->GetSpeed(), 0.0f, player->GetSpeed());
	player->SetPosition(pos);

	//// 移動ベクトルの正規化
	//inputVel.Normalized();

	//// 座標の更新
	//// 現在の位置 + 正規化ベクトル * 移動速度
	//Vector4 pos = player->GetPosition();
	//pos = pos + inputVel * player->GetSpeed();
	//player->SetPosition(pos);

	//// 向きの更新(Y軸回転)
	//Vector4 rot = player->GetRotation();
	//rot.Set(0.0f, std::atan2f(inputVel.GetX(), inputVel.GetZ()), 0.0f);
	//player->SetRotation(rot);
}

// ジャンプ状態
void PlayerStateJump::Enter(Player* player)
{
	// ジャンプ開始時に上方向の初速を設定
	_verticalVelocity = 20.0f;

	player->PlayAnimation("mainRig|JumpUp", 10.0f, 1, 0.5f);
}

void PlayerStateJump::Update(Player* player)
{
	Vector4 pos = player->GetPosition();
	Vector4 inputVel = player->GetInputVelocity();

	// 重力を加算
	_verticalVelocity -= 0.98f;

	// 座標の更新
	// X : 空中制御、Y : ジャンプと落下、Z : 常に前進
	pos = pos + Vector4(inputVel.GetX() * player->GetSpeed(), _verticalVelocity, player->GetSpeed());

	// 着地判定(0.0f以下になったら地面とみなす)
	if (pos.GetY() <= 0.0f)
	{
		pos.SetY(0.0f);
		player->SetPosition(pos);
		player->ChangeState(std::make_unique<PlayerStateIdle>());
		return;
	}

	player->SetPosition(pos);
}

void PlayerStateJump::Exit(Player* player)
{
	// ジャンプ終了時の処理
	player->PlayAnimation("mainRig|JumpDown", 10.0f, 1, 0.5f);
}

// ローリング状態
void PlayerStateRoll::Enter(Player* player)
{
	// ローリング開始時の処理
	_timer = 0.0f;
	player->SetRollingCollider();// コライダーをローリング用に変更
	player->PlayAnimation("mainRig|Rolling", 10.0f, 1, 0.5f);
}

void PlayerStateRoll::Update(Player* player)
{
	_timer += 1.0f;
	if (_timer >= 70.0f)// アニメーション時間
	{
		// ローリング終了
		player->ChangeState(std::make_unique<PlayerStateIdle>());
		return;
	}

	// 常に前進
	Vector4 pos = player->GetPosition();
	pos = pos + Vector4(0.0f, 0.0f, player->GetSpeed());
	player->SetPosition(pos);
}

void PlayerStateRoll::Exit(Player* player)
{
	// ローリング終了時の処理
	player->ResetCollider();// コライダーを元に戻す
}
