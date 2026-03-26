#include "UIDamageFlash.h"
#include "CharacterBase.h"

UIDamageFlash::UIDamageFlash(CharacterBase* target, float screenWidth, float screenHeight)
	: UIElement(0.0f, 0.0f, screenWidth, screenHeight)
	, _target(target)
	, _screenWidth(screenWidth)
	, _screenHeight(screenHeight)
	, _currentAlpha(0.0f)
	, _prevHp(-1.0f)
{
	if (_target)
	{
		_target->AddObserver(this);
		_prevHp = _target->GetHP();
	}
}

UIDamageFlash::~UIDamageFlash()
{
	if (_target)
	{
		_target->RemoveObserver(this);
	}
}

void UIDamageFlash::Process()
{
	if (_isDead) { return; }

	// アルファ値を徐々に減らす(フェードアウト)
	if (_currentAlpha > 0.0f) 
	{
		_currentAlpha -= 5.0f;

		if (_currentAlpha < 0.0f) 
		{
			_currentAlpha = 0.0f;
		}
	}
}

void UIDamageFlash::Render()
{
	// アルファ値が0以下のときは描画処理をスキップ
	if (_isDead || _currentAlpha <= 0.0f) { return; }

	// アルファブレンドを有効化
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(_currentAlpha));

	// 単色の赤で画面全体を覆う
	DrawBox(0, 0, static_cast<int>(_screenWidth), static_cast<int>(_screenHeight), GetColor(255, 0, 0), TRUE);

	// ブレンドモードを通常に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIDamageFlash::OnHPChanged(float currentHp, float maxHp)
{
	// HPが前回より減っていた場合
	if (_prevHp > 0.0f && currentHp < _prevHp) 
	{
		// 不透明度をガッツリ上げる
		_currentAlpha = 150.0f;
	}

	// 今回のHPを記憶
	_prevHp = currentHp;
}

void UIDamageFlash::OnTargetDestroyed()
{
	// ターゲットが破棄されたら安全のためにポインタを外す
	_target = nullptr;
}