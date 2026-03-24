#pragma once
#include "GameObject.h"
class LaserCollision : public GameObject
{
public:
	LaserCollision();
	virtual ~LaserCollision();
	virtual void Process(int key, int trg) override;
	virtual void Render() override;

	void RenderShadow();

	VECTOR GetStartVPos() const { return _startVPos; }
	VECTOR GetEndVPos() const { return _endVPos; }

	void SetStartPos(const VECTOR& startPos) { _startVPos = startPos; }
	void SetEndPos(const VECTOR& endPos) { _endVPos = endPos; }

protected:
	VECTOR _startVPos;//レーザーの始点
	VECTOR _endVPos;//レーザーの終点
	float _circleHeight;//レーザーの厚さ

	// シャドウ用のMV1モデル作成
	int _grHandle = -1;// テクスチャとして使用するグラフィックハンドル
	float _modelBaseLength;// モデルのZ方向の基準の長さ
	float _modelBaseHalfWidth;// モデルのX方向の基準の半分の幅

	float _alphaTimer = 0.0f;// レーザー点滅用
};

