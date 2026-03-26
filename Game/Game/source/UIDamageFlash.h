#pragma once
#include "UIElement.h"
#include "IHPObserver.h"

class CharacterBase;

class UIDamageFlash : public UIElement, public IHPObserver
{
public:
	UIDamageFlash(CharacterBase* target, float screenWidth, float screenHeight);
	virtual ~UIDamageFlash();

	void Initialize() override {}
	void Process() override;
	void Render() override;
	void Terminate() override {}

	void OnHPChanged(float currentHp, float maxHp) override;
	void OnDied() override {}
	void OnTargetDestroyed() override;

private:
	CharacterBase* _target;

	float _screenWidth;
	float _screenHeight;

	float _currentAlpha;
	float _prevHp;
};

