#include "CharacterBase.h"

CharacterBase::CharacterBase(const char* name) 
	: GameComposite(name)
	, _hp(100.0f)
	, _speed(1.0f)
	, _modelHandle(-1)
{
}

void CharacterBase::Initialize()
{
}

void CharacterBase::Process()
{
	if (_isDead) { return; }

	Move();

	// 子のProcessを呼び出す
	GameComposite::Process();
}

void CharacterBase::Render()
{
	if (_isDead) { return; }

	if (_modelHandle != -1)
	{
		// モデルの描画
		// 拡大行列を作成
		MATRIX mScale = MGetScale(ToDX(_scale));

		// 回転行列を作成
		MATRIX mRotX = MGetRotX(_rot.GetX());
		MATRIX mRotY = MGetRotY(_rot.GetY());
		MATRIX mRotZ = MGetRotZ(_rot.GetZ());

		// 平行移動行列を作成
		MATRIX mTrans = MGetTranslate(ToDX(_pos));

		// 拡大 -> 回転 -> 平行移動の順に行列を掛け合わせる
		MATRIX m = MGetIdent();
		m = MMult(m, mScale);
		m = MMult(m, mRotX);// X, Y, Zの順で回転する
		m = MMult(m, mRotY);
		m = MMult(m, mRotZ);
		m = MMult(m, mTrans);// 最後に移動する

		// モデルに行列をセット
		MV1SetMatrix(_modelHandle, m);

		// モデルの描画
		MV1DrawModel(_modelHandle);
	}

	// test
	DrawSphere3D(ToDX(_pos), 10.0f, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

	// 子のRenderを呼び出す
	GameComposite::Render();
}

void CharacterBase::Terminate()
{
	if (_modelHandle != -1)
	{
		// モデルの解放
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

void CharacterBase::TakeDamage(float damage)
{
	if (_isDead) { return; }

	_hp -= damage;
	if (_hp <= 0.0f)
	{
		_hp = 0.0f;
		Destroy();// 破棄フラグを立てる
	}
}
