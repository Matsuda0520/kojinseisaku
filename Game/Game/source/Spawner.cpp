#include "Spawner.h"
#include "GameObjectFactory.h"
#include "GameComposite.h"

Spawner::Spawner(GameComposite* rootNode)
	: _rootNode(rootNode)
{
}

GameObject* Spawner::SpawnPlayer(const std::string& playerName, const Vector4& pos)
{
	if (!_rootNode) { return nullptr; }

	// ファクトリーからunique_ptrでオブジェクトを生成する
	std::unique_ptr<GameObject> player = GameObjectFactory::GetInstance().Create(playerName);
	if (!player) { return nullptr; }

	player->Initialize();
	player->SetPosition(pos);

	// 生ポインタを取得してから所有権を渡す
	GameObject* playerPtr = player.get();
	_rootNode->AddChild(std::move(player));

	return playerPtr;
}
