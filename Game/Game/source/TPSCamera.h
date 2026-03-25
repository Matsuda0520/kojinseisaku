#pragma once
#include "CameraBase.h"

class GameObject;

class TPSCamera : public CameraBase
{
public:
	TPSCamera();

	void Process() override;

	// プレイヤーを登録
	void SetTarget(GameObject* target);

	// パラメータ調整
	void SetOffset(float distance, float height, float lookAtHeight);

private:
	GameObject* _target;

	// カメラの相対位置
	float _distance;// プレイヤーからの距離
	float _height;// プレイヤーからの高さ
	float _lookAtHeight;// プレイヤーのどこを見るか

	// 追従Y固定(ジャンプ時の画面揺れを防止)
	float _fixedTargetY;
	bool _isFixedTargetYInit;
};

