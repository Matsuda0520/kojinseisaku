#pragma once
#include "GameComposite.h"

class GameObject;
class Floor;
class Laser;

// ステージ全体を管理するクラス。ステージのルートノードとして機能する
class StageSpawner : public GameComposite
{
public:
	StageSpawner(const char* name = "StageSpawner");
	~StageSpawner() override = default;

	void Process() override;

	// プレイヤーを登録
	void SetTarget(GameObject* target) { _target = target; }

	// ステージに必要なオブジェクトを生成し、自身の子として追加する
	void BuildInitialStage();

private:
	// プールから非アクティブなレーザーを取得する
	Laser* GetInactiveLaserFromPool();

	GameObject* _target;// プレイヤーへの参照

	std::deque<Floor*> _activeFloors;// 床のプール
	std::vector<Laser*> _laserPool;// レーザーのプール

	float _laserSpawnTimer;// レーザーの再配置タイマー
	std::vector<float> _laserHeightPresets;// レーザーの高さプリセット

	float _nextSpawnZ;
};

