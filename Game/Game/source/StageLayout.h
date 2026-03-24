#pragma once
#include "appframe.h"

#include <vector>

struct LaserData {
	VECTOR start;// レーザーカプセルの始点
	VECTOR end;// レーザーカプセルの終点
	float radius;// レーザーカプセルの半径
};

struct StageLayout {
	const char* stageName;// ステージ名
	// 壁
	float wallThickness;// 壁の厚さ
	float wallHeight;// 壁の高さ
	float wallDistanceX;// 壁の間隔(X方向)
	float wallLengthZ;// 壁の長さ(Z方向)
	// レーザー
	std::vector<LaserData> lasers;// レーザーデータリスト
};


namespace {
	constexpr auto DEFAULT_WALL_THICKNESS = 10.0f;
	constexpr auto DEFAULT_WALL_HEIGHT = 500.0f;
	constexpr auto DEFAULT_WALL_DISTANCE_X = 300.0f;
	constexpr auto DEFAULT_WALL_LENGTH_Z = 3000.0f;
	constexpr auto HORIZON_LASER_R = 8.0f;
	constexpr auto VERTICAL_LASER_R = 8.0f;
}

// レーザー位置のx,yパターンの列挙
enum class LaserXYPattern {
	Y_LOW,
	Y_HIGH,
	RIGHT_UP,
	LEFT_UP,
};

// x,yパターンにzを設定してレーザーデータを作成する関数
inline LaserData CreateLaser(LaserXYPattern pattern, float z) {
	switch (pattern) {
		case LaserXYPattern::Y_LOW:
			return { VGet(-300.0f,100.0f,z), VGet(300.0f,100.0f,z), HORIZON_LASER_R };
		case LaserXYPattern::Y_HIGH:
			return { VGet(-300.0f,200.0f,z), VGet(300.0f,200.0f,z), HORIZON_LASER_R };
		case LaserXYPattern::RIGHT_UP:
			return { VGet(-300.0f,50.0f,z), VGet(300.0f,250.0f,z), VERTICAL_LASER_R };
		case LaserXYPattern::LEFT_UP:
			return { VGet(-300.0f,250.0f,z), VGet(300.0f,50.0f,z), VERTICAL_LASER_R };
		default:
			return { VGet(0.0f,0.0f,z), VGet(0.0f,0.0f,z), 0.0f };
	}
}

// ステージを種類ごとに設定する
// 壁の厚さ、壁の高さ、壁の間隔、壁の長さ
// レーザーデータ（始点、終点、半径）
static const StageLayout STAGE_TEMPLATES[] = {
	{ "stage01", DEFAULT_WALL_THICKNESS, DEFAULT_WALL_HEIGHT, DEFAULT_WALL_DISTANCE_X, DEFAULT_WALL_LENGTH_Z, {
		CreateLaser(LaserXYPattern::Y_LOW, -300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -800.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -1300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -1800.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -2300.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -2800.0f)
	}},
	{ "stage02", DEFAULT_WALL_THICKNESS, DEFAULT_WALL_HEIGHT, DEFAULT_WALL_DISTANCE_X, DEFAULT_WALL_LENGTH_Z, {
		CreateLaser(LaserXYPattern::Y_LOW, -300.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -800.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -1300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -1800.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -2300.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -2800.0f)
	}},
	{ "stage03", DEFAULT_WALL_THICKNESS, DEFAULT_WALL_HEIGHT, DEFAULT_WALL_DISTANCE_X, DEFAULT_WALL_LENGTH_Z, {
		CreateLaser(LaserXYPattern::RIGHT_UP, -300.0f),
		CreateLaser(LaserXYPattern::LEFT_UP, -800.0f),
		CreateLaser(LaserXYPattern::RIGHT_UP, -1300.0f),
		CreateLaser(LaserXYPattern::LEFT_UP, -1800.0f),
		CreateLaser(LaserXYPattern::RIGHT_UP, -2300.0f),
		CreateLaser(LaserXYPattern::LEFT_UP, -2800.0f)
	}},
	{ "stage04", DEFAULT_WALL_THICKNESS, DEFAULT_WALL_HEIGHT, DEFAULT_WALL_DISTANCE_X, DEFAULT_WALL_LENGTH_Z, {
		CreateLaser(LaserXYPattern::RIGHT_UP, -300.0f),
		CreateLaser(LaserXYPattern::Y_HIGH, -800.0f),
		CreateLaser(LaserXYPattern::LEFT_UP, -1300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -1800.0f),
		CreateLaser(LaserXYPattern::RIGHT_UP, -2300.0f),
		CreateLaser(LaserXYPattern::LEFT_UP, -2800.0f)
	}},
	{ "stage05", DEFAULT_WALL_THICKNESS, DEFAULT_WALL_HEIGHT, DEFAULT_WALL_DISTANCE_X, DEFAULT_WALL_LENGTH_Z, {
		CreateLaser(LaserXYPattern::Y_LOW, -300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -800.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -1300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -1800.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -2300.0f),
		CreateLaser(LaserXYPattern::Y_LOW, -2800.0f)
	}},
};

// ステージの種類数を計算
static const int STAGE_TEMPLATE_COUNT = sizeof(STAGE_TEMPLATES) / sizeof(STAGE_TEMPLATES[0]);

// ステージ配列の中からランダムに１つステージを選ぶ
inline const StageLayout& GetRandomStageStageLayout(){
	int index = rand() % STAGE_TEMPLATE_COUNT;
	return STAGE_TEMPLATES[index];
}


