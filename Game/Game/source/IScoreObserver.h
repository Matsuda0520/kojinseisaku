#pragma once

class IScoreObserver
{
public:
	virtual ~IScoreObserver() = default;

	// ƒXƒRƒA‚ª•Ï‰»‚µ‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚é
	virtual void OnScoreChanged(int score) {}
};
