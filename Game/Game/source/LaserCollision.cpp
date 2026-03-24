#include "LaserCollision.h"

// シャドウ用のレーザーモデルを板ポリゴンで作成する
// ローカル座標系 : X = 幅、Y = 上方向(法線)、Z = 長さ
// length : 板ポリゴン全長(Z方向)、halfWidth : 板ポリゴン半幅(X方向)、
// 生成されたモデルのハンドルを返す
static int CreateLaserMV1(float length, float halfWidth, int grHandle)
{
	// 頂点データの初期化(4頂点)
	VERTEX3D v[4] = { 0 };

	// 各頂点の共通設定
	for (int i = 0; i < 4; ++i)
	{
		v[i].dif = GetColorU8(255, 255, 255, 255);// 頂点ディフューズカラー
		v[i].spc = GetColorU8(0, 0, 0, 0);// 頂点スペキュラカラー
		v[i].su = 0.0f;// 補助テクスチャU座標(未使用)
		v[i].sv = 0.0f;// 補助テクスチャV座標(未使用)
		v[i].norm = VGet(0.0f, 1.0f, 0.0f);// 法線(上向き)
	}

	// モデル中心を原点とした、半長さ
	const float hz = length * 0.5f;

	// 4頂点の配置とUV
	// ローカルXに幅(±halfWidth), ローカルZに長さ(±hz)
	// U : 幅方向(左0→右1), V : 長さ方向(手前0→奥1)
	v[0].pos = VGet(-halfWidth, 0.0f, -hz); v[0].u = 0.0f; v[0].v = 0.0f;
	v[1].pos = VGet(halfWidth, 0.0f, -hz); v[1].u = 1.0f; v[1].v = 0.0f;
	v[2].pos = VGet(-halfWidth, 0.0f, hz); v[2].u = 0.0f; v[2].v = 1.0f;
	v[3].pos = VGet(halfWidth, 0.0f, hz); v[3].u = 1.0f; v[3].v = 1.0f;

	// 表面と裏面の2面分の三角形(2 * 3 * 2)
	// 両面描画にするため、表裏で頂点の順番を逆にしている
	unsigned int idx[12] =
	{
		0, 1, 2,  2, 1, 3,// 表(時計回り)
		2, 1, 0,  3, 1, 2 // 裏(反時計回り)
	};

	// マテリアル設定
	MATERIALPARAM mat;
	mat.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);// 基本色
	mat.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);// 反射なし
	mat.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);// 環境光なし
	mat.Emissive = GetColorF(1.0f, 1.0f, 1.0f, 1.0f); // 発光
	mat.Power = 0.0f;// スペキュラの鋭さ(未使用)

	// モデルの生成
	// grHandle >= 0 ならそのテクスチャを貼る。-1 なら無
	return MV1CreateSimpleModel(v, 4, idx, 12, &mat, grHandle);
}

LaserCollision::LaserCollision() {
	_collRadius = 5.0f;
	_startVPos = VGet(-300.0f, 100.0f, -900.0f);
	_endVPos = VGet(300.0f, 100.0f, -900.0f);
	_circleHeight = .0f;

	// モデル作成
	_modelBaseLength = 2000.0f;
	_modelBaseHalfWidth = 1.0f;
	_handle = CreateLaserMV1(_modelBaseLength, _modelBaseHalfWidth, -1);
}

LaserCollision::~LaserCollision() {
}

void LaserCollision::Process(int key, int trg) {
	_alphaTimer += 0.05f;
}

void LaserCollision::Render()
{
	SetUseLighting(FALSE);

	// アルファ値をsin波で変化させる
	float alpha = 0.5f + 0.5f * sinf(_alphaTimer);// 0.0f～1.0fに収める
	int a = static_cast<int>(alpha * 255.0f);

	// 中心のレーザー
	DrawCapsule3D(_startVPos, _endVPos, _collRadius * 0.5, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);

	// 内側の光
	SetDrawBlendMode(DX_BLENDMODE_ADD, 192);
	DrawCapsule3D(_startVPos, _endVPos, _collRadius, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 外側の光
	SetDrawBlendMode(DX_BLENDMODE_ADD, a / 2);
	DrawCapsule3D(_startVPos, _endVPos, _collRadius * 1.5f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetUseLighting(TRUE);
}

// シャドウマップ用のレーザーモデルを描画する
void LaserCollision::RenderShadow()
{
	// レーザーの始点・終点から方向ベクトルと長さを計算
	VECTOR dir = VSub(_endVPos, _startVPos);
	float len = VSize(dir);

	if(len < 0.0001f) { return; }// 長さゼロなら描画しない

	// 方向ベクトルを正規化
	VECTOR n = VScale(dir, 1.0f / len);

	// ローカルZ(0,0,1)を n に向けるためのヨー・ピッチ角
	float yaw = atan2f(n.x, n.z);// 水平回転(Y軸周り)
	float pitch = -asinf(n.y);// 上下回転(X軸周り)

	// レーザーの中心位置(始点と終点の中点)
	VECTOR center = VScale(VAdd(_startVPos, _endVPos), 0.5f);

	// モデルの位置・回転設定
	MV1SetPosition(_handle, center);
	MV1SetRotationXYZ(_handle, VGet(pitch, yaw, 0.0f));

	// スケール設定
	// X : 基準半幅を現在の半径に、Z : 基準長さを現在の長さに合わせる
	float scaleX = (_modelBaseHalfWidth > 0.0f) ? (_collRadius / _modelBaseHalfWidth) : 1.0f;
	float scaleZ = (_modelBaseLength > 0.0f) ? (len / _modelBaseLength) : 1.0f;
	MV1SetScale(_handle, VGet(scaleX, 1.0f, scaleZ));// Y は1.0fのまま

	// モデル描画
	MV1DrawModel(_handle);
}