#include "ModeBase.h"

// 時間経過をさせる
void ModeBase::StepTime(unsigned long tmNow)
{
	// 時間経過処理
    if (_cntMode == 0) 
    {
        _tmMode = 0;
        _tmStep = 0;
        _tmModeBase = tmNow;
        _tmPauseBase = 0;
        _tmPauseStep = 0;
    }
    else 
    {
        _tmMode = tmNow - _tmModeBase + _tmPauseStep;
        _tmStep = tmNow - _tmOldFrame;
    }
    _tmOldFrame = tmNow;
}

// カウントを進める
void ModeBase::StepCount()
{
    _cntMode++;
}