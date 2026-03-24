#include "StageManager.h"
#include "ModeGame.h"
#include "LaserCollision.h"

StageManager::StageManager(const StageLayout* layout, float originZ)
	: _layout(layout), _originZ(originZ) {
	_planeGrHandle = LoadGraph("res/Ground/grass.png");
	_wallGrHandle = LoadGraph("res/Ground/dirt.png");
}

void StageManager::Build(ModeGame* game) {
	if (!_layout || !game) return;

	// リスト内のレーザー数、レイアウトのレーザー数を保存
	_laserStartIndex = game->GetLaserCnt();
	_laserCnt = _layout->lasers.size();

	// レーザー設定から、オブジェクトを作成し、追加
	for (int i = 0; i < _layout->lasers.size(); ++i) {
		const LaserData& ld = _layout->lasers[i];
		auto laser = std::make_shared<LaserCollision>();
		// 始点と終点にはつくられたステージのZ座標基点を加算する
		VECTOR start = VGet(ld.start.x, ld.start.y, _originZ + ld.start.z);
		VECTOR end = VGet(ld.end.x, ld.end.y, _originZ + ld.end.z);
		laser->SetStartPos(start);
		laser->SetEndPos(end);
		laser->SetCollRadius(ld.radius);
		game->AddLaserCollision(laser);
	}
}

void StageManager::RenderWalls() {
	if (!_layout) return;
	// ステージの手前(originZ)から、Z軸負方向にlength分伸ばす
	float  zStart = _originZ;
	float zEnd = _originZ - _layout->wallLengthZ;

	float t = _layout->wallThickness;// 壁の厚さ(1/2)
	float h = _layout->wallHeight;// 壁の高さ
	float d = _layout->wallDistanceX;// 真ん中からの壁の間隔

	// 床
	if (!_floorCreate) {
		auto tileSize = 100.0f;
		// ステージのx,zサイズからタイル数を計算
		auto stageWidth = 2.0f * (d + t);
		auto stageDepth = (zStart - zEnd);
		auto tileX = static_cast<int>(stageWidth / tileSize);
		auto tileZ = static_cast<int>(stageDepth / tileSize);
		// 平面を生成する原点位置を計算
		VECTOR centerPos = VGet(0.0f, 0.0f, (zStart + zEnd) * 0.5f);
		// 平面プリミティブを作成
		_floorPlane.CreatePlane(_planeGrHandle, centerPos, tileSize, tileX, tileZ);
		_floorCreate = true;
	}
	// ライトの向きとポリゴンの向きがずれていたら暗くする処理を行うかどうか
	SetUseLightAngleAttenuation(FALSE);
	_floorPlane.Render();
	SetUseLightAngleAttenuation(TRUE);

	// X : 壁の間隔に厚さ分を足した位置、Y : 0、Z : zStart ~ zEnd
	// 右壁
	VECTOR p1_right = VGet(-d - t, 0.0f, zStart);
	VECTOR p2_right = VGet(-d + t, h, zEnd);
	// 左壁
	VECTOR p1_left = VGet(d - t, 0.0f, zStart);
	VECTOR p2_left = VGet(d + t, h, zEnd);
	// 壁
	if (!_wallCreate) {
		_rightWallCube.CreateCube(_wallGrHandle, p1_right, p2_right);
		_leftWallCube.CreateCube(_wallGrHandle, p1_left, p2_left);
		_wallCreate = true;
	}
	SetUseLighting(FALSE);
	_rightWallCube.Render();
	_leftWallCube.Render();
	SetUseLighting(TRUE);
}