#pragma once
#include "GameObject.h"

// 子を持つオブジェクト
class GameComposite : public GameObject
{
public:
	GameComposite(const char* name) : GameObject(name) {}

	void Initialize() override;
	void Terminate() override;
	void Process() override;
	void Render() override;

	// 子オブジェクトの追加。親を設定して、追加予約用コンテナに追加する
	void AddChild(std::unique_ptr<GameObject> child) override;

private:
	// 子オブジェクトのコンテナ
	std::vector<std::unique_ptr<GameObject>> _children;

	// 追加予約用コンテナ。次のフレームで_childrenに移す
	std::vector<std::unique_ptr<GameObject>> _pendingChildren;

};

