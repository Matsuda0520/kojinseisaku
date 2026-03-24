#pragma once
#include "GameComposite.h"

class GameObject;
class Floor;

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
	GameObject* _target;// プレイヤーへの参照

	std::deque<Floor*> _activeFloors;// オブジェクトプール用のリスト
	float _nextSpawnZ;
};

