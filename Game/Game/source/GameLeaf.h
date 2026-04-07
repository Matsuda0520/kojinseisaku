#pragma once
#include "GameObject.h"

// 子を持たないオブジェクト
class GameLeaf : public GameObject
{
public:
	GameLeaf(const char* name) : GameObject(name) {}

	// Leafは、自身のコンポーネントの処理を呼ぶだけ
	void Initialize() override { GameObject::Initialize(); }
	void Terminate() override { GameObject::Terminate(); }
	void Process() override { GameObject::Process(); }
	void Render() override { GameObject::Render(); }

};

