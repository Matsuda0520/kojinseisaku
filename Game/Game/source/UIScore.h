#pragma once
#include "UIElement.h"
#include "IScoreObserver.h"

class ModeGame;

class UIScore : public UIElement, public IScoreObserver
{
public:
	UIScore(ModeGame* targetMode, float x, float y);
	virtual ~UIScore();

	void Initialize() override {}
	void Process() override;
	void Render() override;
	void Terminate() override {}

	void OnScoreChanged(int score) override;

private:
	ModeGame* _targetMode;

	int _targetScore;// 目標スコア
	float _currentScore;// 現在描画中のスコア
};

