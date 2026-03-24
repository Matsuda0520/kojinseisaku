#pragma once
#include "DxLib.h"
#include <vector>

// 3Dプリミティブの基底クラス
class PrimitiveBase
{
public:
	PrimitiveBase() : _grHandle(-1) {}
	virtual ~PrimitiveBase() { Clear(); }

	// 共通の描画処理
	virtual void Render();

	// データのクリア
	virtual void Clear();

	// テクスチャハンドルの設定
	void SetTexture(int grHandle) { _grHandle = grHandle; }

protected:
	std::vector<VERTEX3D> _vertices;// 頂点データ
	std::vector<unsigned short> _indices;// インデックスデータ
	int _grHandle;// テクスチャハンドル

};

