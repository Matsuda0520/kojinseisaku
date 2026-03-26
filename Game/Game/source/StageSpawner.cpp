#include "StageSpawner.h"
#include "Floor.h"
#include "Laser.h"
#include "NormalLaser.h"
#include "MovingLaser.h"
#include "RotatingLaser.h"

namespace
{
	// 床関連
	constexpr float FLOOR_LENGTH_Z = 1000.0f;// 1枚の床の長さ
	constexpr float POOL_SIZE = 5;// 存在する床の数
	constexpr float DELETE_DISTANCE = 1500.0f;// プレイヤーからこの距離だけ離れた床は再配置

	// レーザー関連
	constexpr int LASER_POOL_SIZE = 10;// レーザーの総数
	constexpr float LASER_RADIUS = 20.0f;// レーザーの半径
	constexpr float LASER_SPAWN_INTERVAL = 180.0f;// 再配置間隔(フレーム)
	constexpr float LASER_DELETE_DISTANCE = 50.0f;// レーザー再配置用
}

StageSpawner::StageSpawner(const char* name)
	: GameComposite(name)
	, _target(nullptr)
	, _nextSpawnZ(-FLOOR_LENGTH_Z)// 最初の床は少し前から
	, _laserSpawnTimer(0.0f)
{
	// レーザーの高さプリセットを設定
	_laserHeightPresets = { 
		50.0f, // しゃがみ
		150.0f, // 通常
		250.0f // ジャンプ
	};
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

	// レーザーのタイマー再配置
	_laserSpawnTimer += 1.0f;

	// 再配置間隔に達したらレーザーを配置する
	if (_laserSpawnTimer >= LASER_SPAWN_INTERVAL)
	{
		_laserSpawnTimer = 0.0f;

		Laser* laser = GetInactiveLaserFromPool();
		if (laser)
		{
			// レーザーをアクティブにする
			laser->Revive();

			// プリセットから抽選
			int index = static_cast<int>(Math::RandomRange(0, _laserHeightPresets.size()));
			float selectHeight = _laserHeightPresets[index];

			// プレイヤーの前方に配置
			float spawnZ = targetZ + 3000.0f;

			// 基準となるY座標とZ座標を渡してセットアップする
			laser->Setup(selectHeight, spawnZ);
		}
	}

	// 画面外に出たレーザーをプールに戻す
	for (auto* laser : _laserPool)
	{
		// 非アクティブなレーザーはスキップ
		if (laser->IsSleeping()) { continue; }

		// 画面の邪魔になるため、レーザーは早めに見えなくする
		if (laser->GetPosition().GetZ() < targetZ - LASER_DELETE_DISTANCE)
		{
			// レーザーを非アクティブにする
			laser->Sleep();
		}
	}
}

void StageSpawner::BuildInitialStage()
{
	int grHandle = LoadGraph("res/Ground/grass.png");

	// 床のプール生成
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

	// レーザーのプール生成
	for(int i = 0; i < LASER_POOL_SIZE; ++i)
	{
		std::unique_ptr<Laser> laser;

		// 各レーザーを指定数で生成してプールに入れる
		if(i < 6) { laser = std::make_unique<NormalLaser>("NormalLaser"); }
		else if(i < 8) { laser = std::make_unique<MovingLaser>("MovingLaser"); }
		else { laser = std::make_unique<RotatingLaser>("RotatingLaser"); }

		// 初期状態は非アクティブにする
		laser->Initialize();
		laser->Sleep();

		// 管理用のポインタを保持
		_laserPool.push_back(laser.get());

		// 所有権は自身の子に渡す
		AddChild(std::move(laser));
	}
}

// プールから非アクティブなレーザーを取得する
Laser* StageSpawner::GetInactiveLaserFromPool()
{
	// 非アクティブなレーザーを一時的に保存するコンテナ
	std::vector<Laser*> inactiveLasers;

	for (auto* laser : _laserPool)
	{
		// 非アクティブなレーザーを探す
		if(laser->IsSleeping())
		{
			inactiveLasers.push_back(laser);
		}	
	}

	// 非アクティブなレーザーがいない場合はnullptrを返す
	if (inactiveLasers.empty()) { return nullptr; }

	// 非アクティブな中からランダムに1つ選ぶ
	int index = static_cast<int>(Math::RandomRange(0, inactiveLasers.size()));
	return inactiveLasers[index];
}
