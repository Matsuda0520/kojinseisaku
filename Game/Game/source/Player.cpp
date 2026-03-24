#include "Player.h"
#include "PlayerState.h"
#include "ActionMap.h"

Player::Player(const char* name)
	: CharacterBase(name)
{
	_speed = 5.0f;

	// 初期状態は待機状態
	_currentState = std::make_unique<PlayerStateIdle>();
}

void Player::Initialize()
{
	CharacterBase::Initialize();

	// モデルの設定
	_modelHandle = MV1LoadModel("res/player.mv1");
	// animマネージャーにモデルをセット
	_animManager.SetModelHandle(_modelHandle);
	_rot.Set(0.0f, Math::PI, 0.0f);// 初期向きは180度回転させる

	// コライダーを生成して、子として追加する
	auto collider = std::make_unique<CapsuleCollider>(
		"PlayerCollider", 
		CollisionLayer::Player, 
		20.0f, 45.0f,// 半径、高さ
		this
	);
	_collider = collider.get();// アクセス用にポインタを保持
	AddChild(std::move(collider));// 所有権は親へ渡す

	// 初期ステートの開始処理
	if (_currentState)
	{
		_currentState->Enter(this);
	}
}

void Player::Process()
{
	if (_isDead) { return; }

	// CharacterBase::Process()でMove()が呼ばれる
	CharacterBase::Process();

	// アニメーションの更新
	_animManager.Update();
}

void Player::Terminate()
{
	// 先にアニメーションをデタッチ
	_animManager.SetModelHandle(-1);
	_animManager.Release();

	CharacterBase::Terminate();
}

void Player::Move()
{
	// 現在の状態のUpdateを呼び出す
	if (_currentState)
	{
		_currentState->Update(this);
	}
}

void Player::TakeDamage(float damage)
{
	if (_isDead) { return; }

	float prevHp = _hp;
	CharacterBase::TakeDamage(damage);

	//// ダメージ通知
	//if(_hp < prevHp)
	//{
	//	NotifyDamage();
	//}

	//// 死亡通知
	//if (_hp <= 0.0f)
	//{
	//	NotifyDeath();
	//}
}

void Player::ChangeState(std::unique_ptr<PlayerState> newState)
{
	// 前の状態の終了処理
	if (_currentState)
	{
		_currentState->Exit(this);
	}

	// 新しい状態に切り替える
	_currentState = std::move(newState);

	// 新しい状態の開始処理
	if (_currentState)
	{
		_currentState->Enter(this);
	}
}

Vector4 Player::GetInputVelocity() const
{
	Vector4 velocity(0.0f, 0.0f, 0.0f);
	auto& input = InputManager::GetInstance();
	auto& actionMap = ActionMap::GetInstance();

	float moveX = 0.0f;

	// アナログスティック入力
	const auto& analog = input.GetAnalog();
	if (std::abs(analog.lx) > 0.1f)
	{
		moveX = analog.lx;
	}
	else
	{
		// キーボード入力
		if(actionMap.IsHold(InputAction::MoveLeft))
		{
			moveX -= 1.0f;
		}
		if(actionMap.IsHold(InputAction::MoveRight))
		{
			moveX += 1.0f;
		}
	}

	velocity.SetX(moveX);// X軸の入力値だけセット
	return velocity;
}

bool Player::PlayAnimation(const char* animName, float blendFrame, int loopCnt, float playSpeed)
{
	return _animManager.ChangeAnimationByName(animName, blendFrame, loopCnt, playSpeed);
}

void Player::OnCollision(GameObject* other)
{
	if (!other) { return; }

	// 相手が攻撃判定を持つなら、ダメージ量を渡す
	IAttacker* attacker = other->AsAttacker();
	if (!attacker) { return; }

	TakeDamage(attacker->GetAttackPower());
}
