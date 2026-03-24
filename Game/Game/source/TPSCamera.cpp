#include "TPSCamera.h"
#include "GameObject.h"

TPSCamera::TPSCamera()
	: _target(nullptr)
	, _distance(-400.0f)
	, _height(200.0f)
	, _lookAtHeight(100.0f)
{
}

void TPSCamera::Process()
{
	// ターゲットがいない、またはターゲットが死亡している場合は処理しない
	if (!_target || _target->IsDead()) { return; }

	// ターゲットの現在座標を取得
	Vector4 targetPos = _target->GetPosition();

	// プレイヤーの後方上にカメラを配置
	_pos = Vector4(
		targetPos.GetX(),
		targetPos.GetY() + _height,
		targetPos.GetZ() + _distance
	);

	// 注視点はプレイヤーの頭上
	_lookAt = Vector4(
		targetPos.GetX(),
		targetPos.GetY() + _lookAtHeight,
		targetPos.GetZ()
	);
}

void TPSCamera::SetOffset(float distance, float height, float lookAtHeight)
{
	_distance = distance;
	_height = height;
	_lookAtHeight = lookAtHeight;
}