#include "StageSpawner.h"
#include "Floor.h"
#include "Laser.h"

namespace
{
	constexpr float FLOOR_LENGTH_Z = 1000.0f;// 1枚の床の長さ
	constexpr float POOL_SIZE = 5;// 存在する床の数
	constexpr float DELETE_DISTANCE = 1500.0f;// プレイヤーからこの距離だけ離れた床は再配置
}

StageSpawner::StageSpawner(const char* name)
	: GameComposite(name)
	, _target(nullptr)
	, _nextSpawnZ(-FLOOR_LENGTH_Z)// 最初の床は少し前から
{
}

void StageSpawner::Process()
{
	// 子のProcessを呼ぶ
	GameComposite::Process();

	// ターゲットがいない、または死亡時、プールが空の場合は処理しない
	if (!_target || _target->IsDead() || _activeFloors.empty()) { return; }

	// 一番古い床を取得
	Floor* oldestFloor = _activeFloors.front();
	if (!oldestFloor) { return; }

	float targetZ = _target->GetPosition().GetZ();

	// プレイヤーの現在Zを基準に、床が削除ラインより後ろなら前方へ再配置する
	if (oldestFloor->GetPosition().GetZ() < targetZ - DELETE_DISTANCE)
	{
		// 新しい位置へ再配置
		oldestFloor->SetPosition(Vector4(0.0f, 0.0f, _nextSpawnZ));

		// 先頭を末尾に移動して、プールの順番を更新
		_activeFloors.pop_front();
		_activeFloors.push_back(oldestFloor);

		// 次の再配置先Z座標を更新
		_nextSpawnZ += FLOOR_LENGTH_Z;
	}
}

void StageSpawner::BuildInitialStage()
{
	int grHandle = LoadGraph("res/Ground/grass.png");

	for (int i = 0; i < POOL_SIZE; ++i)
	{
		auto floor = std::make_unique<Floor>(grHandle, 1000.0f, 1, 1, "Floor");

		floor->SetPosition(Vector4(0.0f, 0.0f, _nextSpawnZ));

		// プールに追加
		_activeFloors.push_back(floor.get());

		// 所有権は自身の子に渡す
		AddChild(std::move(floor));

		// 次の床のZ座標を更新
		_nextSpawnZ += FLOOR_LENGTH_Z;
	}
}
