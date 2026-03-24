#pragma once
#include "GameObject.h"

// 子を持つオブジェクト
class GameComposite : public GameObject
{
public:
	GameComposite(const char* name) : GameObject(name) {}

	void Initialize() override;
	void Process() override;
	void Render() override;
	void Terminate() override;

	// 自身と子をまとめて死亡状態にする
	void Destroy() override;

	// 子オブジェクトを追加する。追加は次のフレームから有効になる
	void AddChild(std::unique_ptr<GameObject> child) override;

private:
	std::vector<std::unique_ptr<GameObject>> _children;
	// 追加予約用コンテナ。次のフレームで_childrenに移す
	std::vector<std::unique_ptr<GameObject>> _pendingChildren;
};

