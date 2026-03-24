#pragma once
#include <unordered_map>
#include <vector>

#include "appframe.h"

// 入力アクション定義
enum class InputAction
{
	MoveForward,// 前進
	MoveBackWard,// 後退
	MoveLeft,// 左移動
	MoveRight,// 右移動
	Jump,// ジャンプ
	_EOT_
};

// 各アクションに対するキーコンフィグ定義
struct ActionBinding
{
	std::vector<int> keys;// 割り当てられたキー
	std::vector<int> padButtons;// 割り当てられたパッドボタン

	// 他の入力フラグ
	bool hatUp = false;
	bool hatRight = false;
	bool hatDown = false;
	bool hatLeft = false;
	bool triggerLt = false;
	bool triggerRt = false;

	ActionBinding& AddKey(int key) { keys.push_back(key); return *this; }
	ActionBinding& AddPad(int button) { padButtons.push_back(button); return *this; }
	ActionBinding& SetHatUp() { hatUp = true; return *this; }
	ActionBinding& SetHatRight() { hatRight = true; return *this; }
	ActionBinding& SetHatDown() { hatDown = true; return *this; }
	ActionBinding& SetHatLeft() { hatLeft = true; return *this; }
	ActionBinding& SetTriggerLt() { triggerLt = true; return *this; }
	ActionBinding& SetTriggerRt() { triggerRt = true; return *this; }
};

class ActionMap
{
public:
	static ActionMap& GetInstance()
	{
		static ActionMap instance;
		return instance;
	}

	// ゲームからはこの関数を呼ぶ
	bool IsHold(InputAction action);
	bool IsTrigger(InputAction action);
	bool IsRelease(InputAction action);

	// 割り当ての動的変更
	void Bind(InputAction action, const ActionBinding& binding);

private:
	ActionMap();
	ActionMap(const ActionMap&) = delete;
	ActionMap& operator=(const ActionMap&) = delete;

	// デフォルトのキーコンフィグを設定
	void InitializeDefaultBindings();

	std::unordered_map<InputAction, ActionBinding> _bindings;
};

