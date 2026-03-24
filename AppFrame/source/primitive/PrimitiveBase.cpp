#include "PrimitiveBase.h"

void PrimitiveBase::Render()
{
	if (_vertices.empty() || _indices.empty()) { return; }

	int vertexNum = static_cast<int>(_vertices.size());
	int polygonNum = static_cast<int>(_indices.size()) / 3;

	// 頂点リストと接続順リストを渡して描画
	DrawPolygonIndexed3D(
		_vertices.data()// 頂点データの配列
		, vertexNum// 頂点の総数
		, _indices.data()// インデックスの配列
		, polygonNum// ポリゴンの数
		, _grHandle// テクスチャハンドル
		, TRUE// 半透明フラグ
	);
}

void PrimitiveBase::Clear()
{
	_vertices.clear();
	_indices.clear();
}
