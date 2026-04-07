#pragma once
#include "ColliderComponent.h"

class CapsuleCollider : public ColliderComponent
{
public:
	CapsuleCollider(CollisionLayer layer, float radius);
	virtual ~CapsuleCollider();

	void Initialize() override;

	// カプセル情報
	Vector4 GetCapsuleStart() const { return _segmentStart; }
	Vector4 GetCapsuleEnd() const { return _segmentEnd; }
	float GetCapsuleRadius() const { return _radius; }

	void SetCapsuleRadius(float radius) { _radius = radius; }
	void SetCapsuleSegment(const Vector4& start, const Vector4& end) { _segmentStart = start; _segmentEnd = end; }

	// 衝突時の処理
	void OnCollisionEnter(GameObject* other) override;
	void OnCollisionStay(GameObject* other) override;
	void OnCollisionExit(GameObject* other) override;

private:
	float _radius;
	Vector4 _segmentStart;// 線分の始点
	Vector4 _segmentEnd;// 線分の終点

};

