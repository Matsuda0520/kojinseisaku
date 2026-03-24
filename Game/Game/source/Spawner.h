#pragma once
#include "appframe.h"

class GameObject;
class GameComposite;

class Spawner
{
public:
	// 親となるルートノードの生ポインタを受け取る
	Spawner(GameComposite* rootNode);
	~Spawner() = default;

	// プレイヤーを生成する
	GameObject* SpawnPlayer(const std::string& playerName, const Vector4& pos);

private:
	GameComposite* _rootNode;// オブジェクトを追加するルートツリー
};

