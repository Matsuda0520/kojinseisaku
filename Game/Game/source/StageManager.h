#pragma once
#include "StageLayout.h"
#include "Primitive_Plane.h"
#include "Primitive_Cube.h"

class ModeGame;

class StageManager
{
public:
	// ステージレイアウトのポインタ、ステージのZ座標基点
	StageManager(const StageLayout* layout, float originZ);

	// ステージを構築する
	// lasersリストに設定したレーザーを追加
	void Build(ModeGame* game);

	// ステージの壁を描画する。レイアウトから情報を取得
	void RenderWalls();

	float GetOriginZ() const { return _originZ; }
	float GetLengthZ() const { return _layout->wallLengthZ; }
	float GetEndZ() const { return _originZ - GetLengthZ(); }
	int GetLaserStartIndex() const { return _laserStartIndex; }
	int GetLaserCnt() const { return _laserCnt; }

	const StageLayout* GetLayout() const { return _layout; }

	// 古いステージを削除した後、lasersリストの変更に合わせてここも更新する
	void AdjustLaserStartIndex(int diff) { _laserStartIndex += diff; }

private:
	const StageLayout* _layout = nullptr;
	float _originZ = 0.0f;// ステージのZ座標基点

	int _laserStartIndex = 0;// ステージごとのレーザーがlasersリスト内で何番目から始まるか
	int _laserCnt = 0;// 設定したステージ内でのレーザー数

	// 床用の平面プリミティブ
	Primitive_Plane _floorPlane;
	bool _floorCreate = false;

	// 壁用のcubeプリミティブ
	Primitive_Cube _rightWallCube;
	Primitive_Cube _leftWallCube;
	bool _wallCreate = false;

	// テクスチャハンドル
	int _planeGrHandle = -1;
	int _wallGrHandle = -1;
};

