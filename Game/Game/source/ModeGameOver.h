#pragma once
#include "appframe.h"

class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SetResultScore(int score) { _resultScore = score; }

protected:
	int _fontHandle = -1;
	int _resultScore = 0;// 最終スコア
};

