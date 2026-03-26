#include "UIScore.h"
#include "ModeGame.h"

UIScore::UIScore(ModeGame* targetMode, float x, float y)
	: UIElement(x, y)
	, _targetMode(targetMode)
	, _targetScore(0)
	, _currentScore(0.0f)
{
	if (_targetMode)
	{
		_targetMode->AddScoreObserver(this);
	}
}

UIScore::~UIScore()
{
	if (_targetMode)
	{
		_targetMode->RemoveScoreObserver(this);
	}
}

void UIScore::Process()
{
	if (_isDead) { return; }

	// スコアのカウントアップ演出(目標値に向かって滑らかに増加)
	if (static_cast<int>(_currentScore) != _targetScore)
	{
		_currentScore += (_targetScore - _currentScore) * 0.1f;// 0.1fは追従スピード

		// 誤差が小さくなったら目標値にピッタリ合わせる
		if (std::abs(_targetScore - _currentScore) < 1.0f) 
		{
			_currentScore = static_cast<float>(_targetScore);
		}
	}
}

void UIScore::Render()
{
	if (_isDead) { return; }

	Vector2 globalPos = GetGlobalPos();
	int x = static_cast<int>(globalPos.GetX());
	int y = static_cast<int>(globalPos.GetY());

	// スコアを描画
	DrawFormatString(x, y, GetColor(255, 255, 255), "SCORE: %08d", static_cast<int>(_currentScore));
}

void UIScore::OnScoreChanged(int score)
{
	_targetScore = score;
}