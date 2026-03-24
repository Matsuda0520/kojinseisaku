#include "PrimitivePlane.h"
#include "../DxUtil/DxUtility.h"
#include <array>

namespace
{
	constexpr COLOR_U8 DEFAULT_DIFFUSE = { 255, 255, 255, 255 };
	constexpr COLOR_U8 DEFAULT_SPECULAR = { 0, 0, 0, 0 };
	constexpr VECTOR DEFAULT_NORMAL = { 0.0f, 1.0f, 0.0f };
}

void PrimitivePlane::CreatePlane(int grHandle, const Vector4& originPos, float tileSize, int tileX, int tileZ)
{
	_grHandle = grHandle;// テクスチャハンドルを保存

	int totalTiles = tileX * tileZ;// タイルの総数
	if (totalTiles <= 0) { return; }

	Clear();// 既存のデータをクリア
	// reserveで追加する予定分の容量を確保しておく
	_vertices.reserve(totalTiles * 4);// 頂点はタイル数×4
	_indices.reserve(totalTiles * 6);// インデックスはタイル数×6(三角形2つ分)

	// タイル全体の幅と奥行きを計算
	float stageWidth = tileSize * static_cast<float>(tileX);
	float stageDepth = tileSize * static_cast<float>(tileZ);

	// 原点を中心にするための開始位置を計算
	float startX = originPos.GetX() - (stageWidth * 0.5f);
	float startZ = originPos.GetZ() - (stageDepth * 0.5f);
	float startY = originPos.GetY();

	// タイルの4頂点の相対座標リスト
	std::array<Vector4, 4> posList = {
		Vector4(0.0f, 0.0f, 0.0f),// 左下
		Vector4(0.0f, 0.0f, tileSize),// 左上
		Vector4(tileSize, 0.0f, 0.0f),// 右下
		Vector4(tileSize, 0.0f, tileSize)// 右上
	};

	// タイルのUV座標リスト
	constexpr std::array<float, 4> uList = { 0.0f, 0.0f, 1.0f, 1.0f };
	constexpr std::array<float, 4> vList = { 1.0f, 0.0f, 1.0f, 0.0f };

	unsigned short offsetIndex = 0;// インデックスのオフセット

	// Z軸とX軸の二重ループで全タイルを配置
	for (int z = 0; z < tileZ; ++z)
	{
		float offsetZ = startZ + tileSize * static_cast<float>(z);// Z方向のオフセット

		for (int x = 0; x < tileX; ++x)
		{
			float offsetX = startX + tileSize * static_cast<float>(x);// X方向のオフセット

			// 正方形を形成する
			for (int i = 0; i < 4; ++i)
			{
				VERTEX3D vertex;

				// 各頂点の相対座標を計算
				Vector4 localPos(
					posList[i].GetX() + offsetX,
					posList[i].GetY() + startY,
					posList[i].GetZ() + offsetZ
				);

				vertex.pos = ToDX(localPos);// VECTORに変換

				vertex.norm = DEFAULT_NORMAL;// 法線は上向き固定
				vertex.dif = DEFAULT_DIFFUSE;// ディフューズカラー
				vertex.spc = DEFAULT_SPECULAR;// スペキュラカラー
				vertex.u = uList[i];// UV座標
				vertex.v = vList[i];
				vertex.su = 0.0f;// 補助UVは未使用
				vertex.sv = 0.0f;

				_vertices.push_back(vertex);// 頂点リストに追加
			}

			// 正方形を2つの三角形で描くためのインデックス
			// 頂点の順番は時計回り
			_indices.push_back(offsetIndex);
			_indices.push_back(offsetIndex + 1);
			_indices.push_back(offsetIndex + 2);

			_indices.push_back(offsetIndex + 2);
			_indices.push_back(offsetIndex + 1);
			_indices.push_back(offsetIndex + 3);

			offsetIndex += 4;// 次の正方形へ
		}
	}
}
