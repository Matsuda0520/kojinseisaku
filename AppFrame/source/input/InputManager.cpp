#include "InputManager.h"

void InputManager::Update()
{
	// キーボード入力
	_prevKeyBuffer = _keyBuffer;
	GetHitKeyStateAll(_keyBuffer.data());

	// パッド入力
	int oldPad = _padKey;
	_padKey = GetJoypadInputState(DX_INPUT_PAD1);
	_padTrg = (_padKey ^ oldPad) & _padKey;
	_padRel = (_padKey ^ oldPad) & ~_padKey;

	// アナログスティック入力
	DINPUT_JOYSTATE di;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &di);
	_analog.lx = static_cast<float>(di.X) / 1000.0f;
	_analog.ly = static_cast<float>(di.Y) / 1000.0f;
	_analog.rx = static_cast<float>(di.Rx) / 1000.0f;
	_analog.ry = static_cast<float>(di.Ry) / 1000.0f;
	_analog.lz = static_cast<float>(di.Z) / 1000.0f;
	_analog.rz = static_cast<float>(di.Rz) / 1000.0f;

	// ハットスイッチ入力
	_hat.oldAngle = _hat.angle;
	_hat.angle = static_cast<int>(di.POV[0]);

	// トリガー入力
	bool prevLtHold = _trigger.ltHold;
	bool prevRtHold = _trigger.rtHold;
	_trigger.lt = static_cast<float>(di.Z) / 1000.0f;
	_trigger.rt = static_cast<float>(-di.Z) / 1000.0f;
	if (_trigger.lt < 0.0f) _trigger.lt = 0.0f;
	if (_trigger.rt < 0.0f) _trigger.rt = 0.0f;
	_trigger.ltHold = (_trigger.lt >= _triggerMin);
	_trigger.rtHold = (_trigger.rt >= _triggerMin);
	_trigger.ltTrg = (!prevLtHold && _trigger.ltHold);
	_trigger.rtTrg = (!prevRtHold && _trigger.rtHold);
	_trigger.ltRel = (prevLtHold && !_trigger.ltHold);
	_trigger.rtRel = (prevRtHold && !_trigger.rtHold);
}

void InputManager::Reset()
{
	// キーボード入力をリセット
	_keyBuffer.fill(0);
	_prevKeyBuffer.fill(0);

	// パッド入力をリセット
	_padKey = 0;
	_padTrg = 0;
	_padRel = 0;

	// アナログスティックをリセット
	_analog = AnalogState{};

	// ハットスイッチをリセット
	_hat = HatState{};

	// トリガーをリセット
	_trigger = TriggerButtonState{};
}

// 入力状況
// キーボード
bool InputManager::IsKeyHold(int key)
{
	if (key < 0 || key >= 256) { return false; }
	return _keyBuffer[key] != 0;
}

bool InputManager::IsKeyTrg(int key)
{
	if (key < 0 || key >= 256) { return false; }
	return _keyBuffer[key] != 0 && _prevKeyBuffer[key] == 0;
}

bool InputManager::IsKeyRel(int key)
{
	if (key < 0 || key >= 256) { return false; }
	return _keyBuffer[key] == 0 && _prevKeyBuffer[key] != 0;
}

// パッド
bool InputManager::IsPadHold(int pad)
{
	return (pad != 0) && ((_padKey & pad) != 0);
}

bool InputManager::IsPadTrg(int pad)
{
	return (pad != 0) && ((_padTrg & pad) != 0);
}

bool InputManager::IsPadRel(int pad)
{
	return (pad != 0) && ((_padRel & pad) != 0);
}
