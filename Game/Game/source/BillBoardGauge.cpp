#include "BillBoardGauge.h"

namespace {
	constexpr COLOR_U8 DEFAULT_DIFFUSE_COLOR = { 0, 255, 0, 255 };
	constexpr COLOR_U8 DEFAULT_SPECULAR_COLOR = { 0, 0, 0, 0 };
	constexpr VECTOR DEFAULT_NORMAL = { 0.0f, 1.0f, 0.0f };
}

BillBoardGauge::BillBoardGauge() {
	_vPos = VGet(0.0f, 0.0f, 0.0f);
	_width = 100.0f;
	_height = 10.0f;
	_valuePercent = 100;
	// 頂点データの初期化
	for (int i = 0; i < 4; i++)
	{
		_vertex[i].dif = DEFAULT_DIFFUSE_COLOR;
		_vertex[i].spc = DEFAULT_SPECULAR_COLOR;
		_vertex[i].su = 0.0f;
		_vertex[i].sv = 0.0f;
		_vertex[i].norm = DEFAULT_NORMAL;
	}
	// インデックスデータの初期化
	_index = {
		0, 1, 2,
		2, 1, 3
	};
}

BillBoardGauge::~BillBoardGauge() {
	_vertex.clear();
	_index.clear();
}

void BillBoardGauge::Process() {
	// カメラ情報の取得
	MATRIX mCameraView = GetCameraViewMatrix();
	VECTOR vCameraRight = VGet(mCameraView.m[0][0], mCameraView.m[1][0], mCameraView.m[2][0]);
	VECTOR vCameraUp = VGet(mCameraView.m[0][1], mCameraView.m[1][1], mCameraView.m[2][1]);

	// 頂点座標の計算
	auto halfWidth = _width * 0.5f;
	auto halfHeight = _height * 0.5f;
	// 左端を基準位置とする
	VECTOR vStartPos = VAdd(_vPos, VScale(vCameraRight, -halfWidth));
	auto currentWidth = _width * static_cast<float>(_valuePercent);
}
