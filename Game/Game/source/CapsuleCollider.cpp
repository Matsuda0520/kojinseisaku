#include "CapsuleCollider.h"
#include "CollisionManager.h"

CapsuleCollider::CapsuleCollider(const char* name, CollisionLayer layer, float radius, GameObject* owner)
	: GameLeaf(name)
	, _layer(layer)
	, _radius(radius)
	, _owner(owner)
	, _segmentStart()
	, _segmentEnd()
{
}

CapsuleCollider::~CapsuleCollider()
{
	// 破棄されるときにCollisionManagerから解除する
	CollisionManager::GetInstance().Unregister(this);
}

void CapsuleCollider::Initialize()
{
	// オーナー位置を設定しておく
	if (_owner)
	{
		_segmentStart = _owner->GetPosition();
		_segmentEnd = _owner->GetPosition();
	}

	// CollisionManagerに登録する
	CollisionManager::GetInstance().Register(this);
}

void CapsuleCollider::OnCollision(GameObject* other)
{
	// オーナーのOnCollisionを呼び出す
	if (_owner)
	{
		_owner->OnCollision(other);
	}
}

void CapsuleCollider::SetCapsuleSegment(const Vector4& start, const Vector4& end)
{
	// カプセル軸を更新
	_segmentStart = start;
	_segmentEnd = end;
}
