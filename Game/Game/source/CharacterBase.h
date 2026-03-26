#pragma once
#include "GameComposite.h"
#include "IHPObserver.h"

class CharacterBase : public GameComposite
{
public:
	CharacterBase(const char* name = "CharacterBase");
	virtual ~CharacterBase();

	virtual void Initialize() override;
	virtual void Process() override;
	virtual void Render() override;
	virtual void Terminate() override;

	virtual void Move() = 0;
	virtual void TakeDamage(float damage);// 被ダメージ処理

	// キャラクター型であることを示す
	virtual CharacterBase* AsCharacter() override { return this; }

	// ゲッター　セッター
	float GetHP() const { return _hp; }
	float GetSpeed() const { return _speed; }

	void SetHP(float hp) { _hp = hp; }
	void SetSpeed(float speed) { _speed = speed; }

	// Observer管理
	void AddObserver(IHPObserver* observer);
	void RemoveObserver(IHPObserver* observer);

protected:
	// Observerへの通知
	void NotifyHPChanged();
	void NotifyDeath();

	float _hp;// 体力
	float _maxHp;// 最大体力
	float _speed;// 移動速度

	int _modelHandle;// モデルハンドル

private:
	std::vector<IHPObserver*> _observers;// オブザーバーリスト
};

