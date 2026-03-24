#pragma once
#include "appframe.h"

// すべてのカメラの親となる抽象クラス
class CameraBase
{
public:
	CameraBase() = default;
	virtual ~CameraBase() = default;

	virtual void Process() = 0;

	Vector4 GetPosition() const { return _pos; }
	Vector4 GetLookAt() const { return _lookAt; }

protected:
	Vector4 _pos;// カメラの座標
	Vector4 _lookAt;// カメラの注視点

};

