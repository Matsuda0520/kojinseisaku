#include "ActionMap.h"

ActionMap::ActionMap()
{
	InitializeDefaultBindings();
}

void ActionMap::Bind(InputAction action, const ActionBinding& binding)
{
	_bindings[action] = binding;
}

// デフォルトのキーコンフィグを設定
void ActionMap::InitializeDefaultBindings()
{
	// 左移動
	Bind(InputAction::MoveLeft, ActionBinding()
		.AddKey(KEY_INPUT_A)
		.AddKey(KEY_INPUT_LEFT)
		.AddPad(PAD_INPUT_LEFT)
	);

	// 右移動
	Bind(InputAction::MoveRight, ActionBinding()
		.AddKey(KEY_INPUT_D)
		.AddKey(KEY_INPUT_RIGHT)
		.AddPad(PAD_INPUT_RIGHT)
	);

	// ジャンプ
	Bind(InputAction::Jump, ActionBinding()
		.AddKey(KEY_INPUT_SPACE)
		.AddPad(PAD_INPUT_1)
	);

	// ローリング
	Bind(InputAction::Roll, ActionBinding()
		.AddKey(KEY_INPUT_LSHIFT)
		.AddPad(PAD_INPUT_2)
	);
}

bool ActionMap::IsHold(InputAction action)
{
	auto it = _bindings.find(action);
	if (it == _bindings.end()) { return false; }// アクションが未定義なら常にfalse

	const auto& bind = it->second;// アクションに割り当てられたキーコンフィグ
	auto& input = InputManager::GetInstance();

	// 割り当てられたキーコンフィグのいずれかが入力されていればtrue
	for (int key : bind.keys)
	{
		if (input.IsKeyHold(key)) { return true; }
	}

	for (int pad : bind.padButtons)
	{
		if (input.IsPadHold(pad)) { return true; }
	}

	if (bind.hatUp && input.GetHat().IsUp()) { return true; }
	if (bind.hatRight && input.GetHat().IsRight()) { return true; }
	if (bind.hatDown && input.GetHat().IsDown()) { return true; }
	if (bind.hatLeft && input.GetHat().IsLeft()) { return true; }
	if (bind.triggerLt && input.GetTrigger().ltHold) { return true; }
	if (bind.triggerRt && input.GetTrigger().rtHold) { return true; }

	return false;
}

bool ActionMap::IsTrigger(InputAction action)
{
	auto it = _bindings.find(action);
	if (it == _bindings.end()) { return false; }

	const auto& bind = it->second;
	auto& input = InputManager::GetInstance();

	for (int key : bind.keys)
	{
		if (input.IsKeyTrg(key)) { return true; }
	}

	for(int pad : bind.padButtons)
	{
		if (input.IsPadTrg(pad)) { return true; }
	}

	if (bind.hatUp && input.GetHat().IsUp() && !input.GetHat().WasUp()) { return true; }
	if (bind.hatRight && input.GetHat().IsRight() && !input.GetHat().WasRight()) { return true; }
	if (bind.hatDown && input.GetHat().IsDown() && !input.GetHat().WasDown()) { return true; }
	if (bind.hatLeft && input.GetHat().IsLeft() && !input.GetHat().WasLeft()) { return true; }
	if (bind.triggerLt && input.GetTrigger().ltTrg) { return true; }
	if (bind.triggerRt && input.GetTrigger().rtTrg) { return true; }

	return false;
}

bool ActionMap::IsRelease(InputAction action)
{
	auto it = _bindings.find(action);
	if (it == _bindings.end()) { return false; }

	const auto& bind = it->second;
	auto& input = InputManager::GetInstance();

	for (int key : bind.keys)
	{
		if (input.IsKeyRel(key)) { return true; }
	}

	for (int pad : bind.padButtons)
	{
		if (input.IsPadRel(pad)) { return true; }
	}

	if (bind.hatUp && !input.GetHat().IsUp() && input.GetHat().WasUp()) { return true; }
	if (bind.hatRight && !input.GetHat().IsRight() && input.GetHat().WasRight()) { return true; }
	if (bind.hatDown && !input.GetHat().IsDown() && input.GetHat().WasDown()) { return true; }
	if (bind.hatLeft && !input.GetHat().IsLeft() && input.GetHat().WasLeft()) { return true; }
	if (bind.triggerLt && input.GetTrigger().ltRel) { return true; }
	if (bind.triggerRt && input.GetTrigger().rtRel) { return true; }

	return false;
}