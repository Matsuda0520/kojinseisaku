#pragma once
#include <array>

#include "DxLib.h"

// アナログスティック状態
struct AnalogState
{
	float lx = 0.0f;
	float ly = 0.0f;
	float rx = 0.0f;
	float ry = 0.0f;
	float lz = 0.0f;
	float rz = 0.0f;
};

// ハットスイッチ状態
struct HatState
{
	int angle = -1;// ハットスイッチの角度、未入力は-1
	int oldAngle = -1;// 前フレームの角度

	bool IsUp() const{ return (angle >= 0 && angle < 2250) || (angle >= 33750); }
	bool IsRight() const{ return (angle >= 6750 && angle < 11250); }
	bool IsDown() const{ return (angle >= 17750 && angle < 20250); }
	bool IsLeft() const{ return (angle >= 24750 && angle < 29250); }
	bool IsNeutral() const{ return angle == -1; }

	bool WasUp() const{ return (oldAngle >= 0 && oldAngle < 2250) || (oldAngle >= 33750); }
	bool WasRight() const{ return (oldAngle >= 6750 && oldAngle < 11250); }
	bool WasDown() const{ return (oldAngle >= 17750 && oldAngle < 20250); }
	bool WasLeft() const{ return (oldAngle >= 24750 && oldAngle < 29250); }
};

// トリガー状態
// DxLibのZ軸、Rz軸を 0.0~1.0 の範囲に正規化
struct TriggerButtonState
{
	float lt = 0.0f;
	float rt = 0.0f;
	bool ltHold = false;
	bool rtHold = false;
	bool ltTrg = false;
	bool rtTrg = false;
	bool ltRel = false;
	bool rtRel = false;
};

class InputManager
{
public:
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	void Update();
	void Reset();// 入力状態をリセット

	// キーボード状態取得
	bool IsKeyHold(int key);
	bool IsKeyTrg(int key);
	bool IsKeyRel(int key);

	// パッド状態取得
	bool IsPadHold(int pad);
	bool IsPadTrg(int pad);
	bool IsPadRel(int pad);

	// 各種ハードウェア状態取得
	const AnalogState& GetAnalog() { return _analog; }
	const HatState& GetHat() { return _hat; }
	const TriggerButtonState& GetTrigger() { return _trigger; }

	float GetAnalogMin() { return _analogMin; }
	void SetAnalogMin(float min) { _analogMin = min; }
	void SetTriggerMin(float min) { _triggerMin = min; }

private:
	InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	// ハードウェア入力状態
	// キーボード入力状態
	std::array<char, 256> _keyBuffer = {};
	std::array<char, 256> _prevKeyBuffer = {};

	// パッド入力状態
	int _padKey = 0;
	int _padTrg = 0;
	int _padRel = 0;

	// アナログスティック状態
	AnalogState _analog;
	float _analogMin = 0.3f;// 閾値

	// ハットスイッチ状態
	HatState _hat;

	// トリガー状態
	TriggerButtonState _trigger;
	float _triggerMin = 0.3f;// 閾値
};

