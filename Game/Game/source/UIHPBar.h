#pragma once
#include "UIElement.h"
#include "IHPObserver.h"

class CharacterBase;

class UIHPBar : public UIElement, public IHPObserver
{
public:
	// 対象のキャラクターへのポインタを受け取る
	UIHPBar(CharacterBase* targetCharacter, float x, float y, float w, float h);
	virtual ~UIHPBar();

	void Initialize() override {}
	void Process() override;
	void Render() override;
	void Terminate() override {}

	// IHPObserverの実装
	void OnHPChanged(float currentHp, float maxHp) override;
	void OnDied() override;
	void OnTargetDestroyed() override;

private:
	CharacterBase* _targetCharacter;
	float _targetHpRate;// Hp割合(0.0f~1.0f)
	float _currentHpRate;// 描画中のHp割合
};

