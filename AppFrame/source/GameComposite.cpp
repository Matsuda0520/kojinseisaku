#include "GameComposite.h"

void GameComposite::Initialize()
{
	// まず自身のコンポーネントを処理する
	GameObject::Initialize();

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

void GameComposite::Terminate()
{
	GameObject::Terminate();

	// 子のTerminateを呼び出す
	for (auto& child : _children)
	{
		child->Terminate();
	}

	// 追加予約の子のTerminateも呼び出す
	for (auto& pendingChild : _pendingChildren)
	{
		pendingChild->Terminate();
	}
}

void GameComposite::Process()
{
	if (_isDead) { return; }

	GameObject::Process();

	// 追加予約された子をコンテナに移す
	for (auto& newChild : _pendingChildren)
	{
		// 初期化してから追加する
		newChild->Initialize();
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

	GameObject::Render();

	// 子のRenderを呼び出す
	for (auto& child : _children)
	{
		child->Render();
	}
}

void GameComposite::AddChild(std::unique_ptr<GameObject> child)
{
	if (child)
	{
		child->SetParent(this);// 親を設定する
		_pendingChildren.push_back(std::move(child));// 所有権を移動
	}
}
