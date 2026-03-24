#include "Floor.h"

Floor::Floor(int grHandle, float tileSize, int tileX, int tileZ, const char* name)
	: GameComposite(name)
	, _grHandle(grHandle)
{
	// PrimitivePlaneを生成
	_plane.CreatePlane(grHandle, Vector4(), tileSize, tileX, tileZ);
}

void Floor::Render()
{
	if (_isDead) { return; }

	// 平行移動行列を作成
	MATRIX mWorld = MGetTranslate(ToDX(_pos));

	// ローカル座標からワールド座標への変換行列をセット
	SetTransformToWorld(&mWorld);

	// (0,0,0)で作った頂点が、_posの位置に描画されるようになる
	_plane.Render();

	// ワールド行列を単位行列に戻す
	MATRIX mIdentity = MGetIdent();
	SetTransformToWorld(&mIdentity);
}
