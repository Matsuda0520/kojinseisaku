#include "UIHPBar.h"
#include "CharacterBase.h"

UIHPBar::UIHPBar(CharacterBase* targetCharacter, float x, float y, float w, float h)
	: UIElement(x, y, w, h)
	, _targetCharacter(targetCharacter)
	, _targetHpRate(1.0f)
	, _currentHpRate(1.0f)
{
	// 自身をオブザーバーとして登録
	if (_targetCharacter)
	{
		_targetCharacter->AddObserver(this);
	}
}

UIHPBar::~UIHPBar()
{
	// ここでオブザーバーを解除する
	if (_targetCharacter)
	{
		_targetCharacter->RemoveObserver(this);
	}
}

void UIHPBar::Process()
{
	if (_isDead) { return; }

	// イージングで滑らかに変化させる
	if (_currentHpRate > _targetHpRate)
	{
		_currentHpRate -= 0.01f;// 徐々に減らす
		if(_currentHpRate < _targetHpRate)
		{
			_currentHpRate = _targetHpRate;
		}
	}
}

void UIHPBar::Render()
{
	if (_isDead) { return; }

	Vector2 globalPos = GetGlobalPos();
	int left = static_cast<int>(globalPos.GetX());
	int top = static_cast<int>(globalPos.GetY());
	int right = left + static_cast<int>(_size.GetX());
	int buttom = top + static_cast<int>(_size.GetY());

	// 背景の黒枠
	DrawBox(left, top, right, buttom, GetColor(0, 0, 0), TRUE);

	// Hpゲージ
	int barRight = left + static_cast<int>(_size.GetX() * _currentHpRate);
	DrawBox(left, top, barRight, buttom, GetColor(0, 255, 0), TRUE);
}

void UIHPBar::OnHPChanged(float currentHp, float maxHp)
{
	if (maxHp > 0.0f)
	{
		_targetHpRate = currentHp / maxHp;
	}
}

void UIHPBar::OnDied()
{
	_targetHpRate = 0.0f;
}

void UIHPBar::OnTargetDestroyed()
{
	// 対象が破棄されたら、nullptrにする
	_targetCharacter = nullptr;
}

