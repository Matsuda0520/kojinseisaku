#include "GameComposite.h"

void GameComposite::Initialize()
{
	if (_isDead) { return; }

	// 追加予約された子をコンテナに移す
	for (auto& newChild : _pendingChildren)
	{
		_children.push_back(std::move(newChild));
	}
	_pendingChildren.clear();

	// 子のInitializeを呼び出す
	for (auto& child : _children)
	{
		child->Initialize();
	}
}

void GameComposite::Process()
{
	if (_isDead) { return; }

	// 追加予約された子をコンテナに移す
	for (auto& newChild : _pendingChildren)
	{
		newChild->Initialize();// 追加された子は初期化してから追加する
		_children.push_back(std::move(newChild));
	}
	_pendingChildren.clear();

	// 無効参照、死亡した子を削除する
	_children.erase(
		std::remove_if(_children.begin(), _children.end(),
			[](const std::unique_ptr<GameObject>& child)
			{
				return !child || child->IsDead();
			}),
		_children.end());

	// 子のProcessを呼び出す
	for (auto& child : _children)
	{
		child->Process();
	}
}

void GameComposite::Render()
{
	if (_isDead) { return; }

	// 子のRenderを呼び出す
	for (auto& child : _children)
	{
		child->Render();
	}
}

void GameComposite::Terminate()
{
	if (_isDead) { return; }

	// 子のTerminateを呼び出す
	for (auto& child : _children)
	{
		child->Terminate();
	}
	for (auto& pendingChild : _pendingChildren)
	{
		pendingChild->Terminate();
	}
}

void GameComposite::Destroy()
{
	// 自身を死亡状態にする
	GameObject::Destroy();

	// 子もまとめて死亡状態にする
	for (auto& child : _children) { child->Destroy(); }
	for (auto& pendingChild : _pendingChildren) { pendingChild->Destroy(); }
}

void GameComposite::AddChild(std::unique_ptr<GameObject> child)
{
	if (child)
	{
		child->SetParent(this);// 親を設定する
		_pendingChildren.push_back(std::move(child));// 所有権を移動
	}
}
