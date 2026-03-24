#pragma once
#include "GameObject.h"

// 子を持たないオブジェクト
class GameLeaf : public GameObject
{
public:
	GameLeaf(const char* name) : GameObject(name) {}

	void Initialize() override {}
	void Process() override;
	void Render() override;
	void Terminate() override {}

};

