#include "TPSCamera.h"
#include "GameObject.h"

TPSCamera::TPSCamera()
	: _target(nullptr)
	, _distance(-450.0f)// プレイヤーからの距離
	, _height(400.0f)// プレイヤーからの高さ
	, _lookAtHeight(150.0f)// プレイヤーのどこを見るか
	, _fixedTargetY(0.0f)
	, _isFixedTargetYInit(false)
{
}

void TPSCamera::Process()
{
	// ターゲットがいない、またはターゲットが死亡している場合は処理しない
	if (!_target || _target->IsDead()) { return; }

	// ターゲットの現在座標を取得
	Vector4 targetPos = _target->GetPosition();

	// 初回にY座標を固定
	if (!_isFixedTargetYInit)
	{
		_fixedTargetY = targetPos.GetY();
		_isFixedTargetYInit = true;
	}

	// プレイヤーの後方上にカメラを配置
	_pos = Vector4(
		targetPos.GetX(),
		_fixedTargetY + _height,
		targetPos.GetZ() + _distance
	);

	// 注視点はプレイヤーの頭上
	_lookAt = Vector4(
		targetPos.GetX(),
		_fixedTargetY + _lookAtHeight,
		targetPos.GetZ()
	);
}

void TPSCamera::SetTarget(GameObject* target)
{
	_target = target;
	_isFixedTargetYInit = false;// Y固定をリセット
}

void TPSCamera::SetOffset(float distance, float height, float lookAtHeight)
{
	_distance = distance;
	_height = height;
	_lookAtHeight = lookAtHeight;
}