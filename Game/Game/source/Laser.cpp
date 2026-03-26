#include "Laser.h"
#include "CollisionManager.h"

Laser::Laser(const char* name)
	: GameComposite(name)
	, _collider(nullptr)
	, _capsuleStart()
	, _capsuleEnd(0.0f, 0.0f, 100.0f)
	, _capsuleRadius(20.0f)
	, _attackPower(10.0f)
	, _isSleeping(false)
	, _baseY(0.0f)
	, _baseZ(0.0f)
{
}

void Laser::Initialize()
{
	GameComposite::Initialize();

	// コライダーを生成して、子として追加する
	auto collider = std::make_unique<CapsuleCollider>(
		"LaserCollider",
		CollisionLayer::Enemy,
		_capsuleRadius,// 半径
		this
	);
	_collider = collider.get();// アクセス用にポインタを保持
	AddChild(std::move(collider));// 所有権は親へ渡す

	// カプセルの初期設定
	if (_collider)
	{
		_collider->SetCapsuleRadius(_capsuleRadius);
		_collider->SetCapsuleSegment(_capsuleStart, _capsuleEnd);

		// 非アクティブ中は当たり判定から外す
		if (_isSleeping)
		{
			CollisionManager::GetInstance().Unregister(_collider);
		}
	}
}

void Laser::Process()
{
	// 死亡状態、非アクティブなら処理しない
	if (_isDead || _isSleeping) { return; }

	GameComposite::Process();
}

void Laser::Render()
{
	// 死亡状態、非アクティブなら描画しない
	if (_isDead || _isSleeping) { return; }

	// DxLibへの変換
	VECTOR startPos = ToDX(_capsuleStart);
	VECTOR endPos = ToDX(_capsuleEnd);

	float time = GetNowCount() * 0.001f;// 秒単位に変換
	float flickerSpeed = 20.0f;// 点滅の速さ
	// sin波で0.0~1.0の値を作る(sinは-1.0~1.0なので、+1して0.0~2.0にしてから*0.5して0.0~1.0にする)
	float flicker = (std::sin(time * flickerSpeed) + 1.0f) * 0.5f;

	int outAlpha = 220 + static_cast<int>(flicker * 10);// アルファ値を点滅させる
	//SetDrawBlendMode(DX_BLENDMODE_ADD, outAlpha);// 加算合成で半透明にする

	// 外側のオーラを描画
	unsigned int outColor = GetColor(255, 0, 0);// 赤色
	DrawCapsule3D(startPos, endPos, _capsuleRadius, 12, outColor, outColor, TRUE);

	//int inAlpha = 240 + static_cast<int>(flicker * 15);// 明るめの値
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, inAlpha);// 加算合成で発光させる
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);// ブレンドモードを元に戻す

	// 内側のコアを描画
	unsigned int inColor = GetColor(255, 69, 0);// 白色
	float coreRadius = _capsuleRadius * (0.3f + flicker * 0.2f);// 小さめの半径で変化
	DrawCapsule3D(startPos, endPos, coreRadius, 12, inColor, inColor, TRUE);

	GameComposite::Render();
}

void Laser::SetCapsule(const Vector4& start, const Vector4& end, float radius)
{
	_capsuleStart = start;
	_capsuleEnd = end;
	_capsuleRadius = radius;

	// カプセルの中心点を計算して座標にセット
	_pos.Set(
		(start.GetX() + end.GetX()) * 0.5f,
		(start.GetY() + end.GetY()) * 0.5f,
		(start.GetZ() + end.GetZ()) * 0.5f
	);

	// コライダー情報も更新する
	if (_collider)
	{
		_collider->SetCapsuleRadius(_capsuleRadius);
		_collider->SetCapsuleSegment(_capsuleStart, _capsuleEnd);
	}
}

void Laser::Sleep()
{
	if (_isSleeping) { return; }
	_isSleeping = true;

	// 非アクティブなら当たり判定マネージャーからも削除する
	if (_collider)
	{
		CollisionManager::GetInstance().Unregister(_collider);
	}
}

void Laser::Revive()
{
	if (!_isSleeping) { return; }
	_isSleeping = false;

	// アクティブなら当たり判定マネージャーに再登録する
	if (_collider)
	{
		CollisionManager::GetInstance().Register(_collider);
	}
}
