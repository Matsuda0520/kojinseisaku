#include "CapsuleCollider.h"
#include "CollisionManager.h"
#include "GameObject.h"

CapsuleCollider::CapsuleCollider(CollisionLayer layer, float radius)
	: ColliderComponent(layer, CollisionShape::Capsule)
	, _radius(radius)
	, _segmentStart()
	, _segmentEnd()
{
}

CapsuleCollider::~CapsuleCollider()
{
	CollisionManager::GetInstance().Unregister(this);
}

void CapsuleCollider::Initialize()
{
	// オーナーの位置を取得
	if (GameObject* owner = GetOwner())
	{
		_segmentStart = owner->GetPosition();
		_segmentEnd = owner->GetPosition();
	}

	CollisionManager::GetInstance().Register(this);
}

void CapsuleCollider::OnCollisionEnter(GameObject* other)
{
	if (GameObject* owner = GetOwner())
	{
		owner->OnCollisionEnter(other);
	}
}

void CapsuleCollider::OnCollisionStay(GameObject* other)
{
	if(GameObject* owner = GetOwner())
	{
		owner->OnCollisionStay(other);
	}
}

void CapsuleCollider::OnCollisionExit(GameObject* other)
{
	if(GameObject* owner = GetOwner())
	{
		owner->OnCollisionExit(other);
	}
}
