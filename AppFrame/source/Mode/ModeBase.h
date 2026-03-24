#pragma once
#include <string>

class ModeServer;

class ModeBase
{
public:
	ModeBase() = default;
	virtual ~ModeBase() = default;

	virtual bool Initialize() { return true; }
	virtual bool Terminate() { return true; }
	virtual bool Process() { return true; }
	virtual bool Render() { return true; }

	void Destroy() { _isDead = true; }// 削除フラグを立てる

	const char* GetName() const { return _szName.c_str(); }
	int GetUid() const { return _uid; }
	bool IsDead() const { return _isDead; }
	int	GetModeCount() const { return _cntMode; }// このモードが始まってからのカウンタ
	unsigned long GetModeTm() const { return _tmMode; }// このモードが始まってからの時間。ms
	unsigned long GetStepTm() const { return _tmStep; }// 前フレームからの経過時間。ms
	int GetCallPerFrame() const { return _callPerFrame; }
	int GetCallOfCount() const { return _callOfCount; }

	void SetCallPerFrame(int frame) { _callPerFrame = _callPerFrameCount = frame; }// 何フレームに1回Process()を呼ぶか(def:1)
	void SetCallOfCount(int count) { _callOfCount = count; }// 1回の呼び出しに何回Process()を呼ぶか(def:1)

private:
	friend class ModeServer;
	// ModeServer用
	std::string _szName;
	int _uid = 0;
	int _layer = 0;
	bool _isDead = false;// 削除フラグ

	void StepTime(unsigned long tmNow);
	void StepCount();

	// モード内処理用
	int _cntMode = 0;// 本モードに入ってからのカウンタ。0スタート
	unsigned long _tmMode;// 本モードに入ってからの時間。ms
	unsigned long _tmStep;// 前フレームからの経過時間。ms
	unsigned long _tmModeBase;// このモードが始まった時間。ms
	unsigned long _tmPauseBase;// ポーズ中のベース時間。ms
	unsigned long _tmPauseStep;// ポーズ中の積算時間。ms 動作中、この値を _tmMode に用いる
	unsigned long _tmOldFrame;// 前フレームの時間。ms

	// CallPerFrame / CallOfCount 用
	int _callPerFrame = 1;
	int _callPerFrameCount = 1;
	int _callOfCount = 1;
};


