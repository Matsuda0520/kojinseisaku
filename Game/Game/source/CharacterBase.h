#pragma once
#include "GameComposite.h"

class CharacterBase : public GameComposite
{
public:
	CharacterBase(const char* name = "CharacterBase");
	virtual ~CharacterBase() = default;

	virtual void Initialize() override;
	virtual void Process() override;
	virtual void Render() override;
	virtual void Terminate() override;

	virtual void Move() = 0;
	virtual void TakeDamage(float damage);// 被ダメージ処理

	// ゲッター　セッター
	float GetHP() const { return _hp; }
	float GetSpeed() const { return _speed; }

	void SetSpeed(float speed) { _speed = speed; }

protected:
	float _hp;// 体力
	float _speed;// 移動速度

	int _modelHandle;// モデルハンドル
};

