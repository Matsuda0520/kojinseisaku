#include "CapsuleCollider.h"
#include "CollisionManager.h"

CapsuleCollider::CapsuleCollider(const char* name, CollisionLayer layer, float radius, float halfHeight, GameObject* owner)
	: GameLeaf(name)
	, _layer(layer)
	, _radius(radius)
	, _halfHeight(halfHeight)
	, _owner(owner)
{
}

CapsuleCollider::~CapsuleCollider()
{
	// ”jŠü‚³‚ê‚é‚Æ‚«‚ÉCollisionManager‚©‚ç‰ðœ‚·‚é
	CollisionManager::GetInstance().Unregister(this);
}

void CapsuleCollider::Initialize()
{
	// CollisionManager‚É“o˜^‚·‚é
	CollisionManager::GetInstance().Register(this);
}

void CapsuleCollider::OnCollision(GameObject* other)
{
	// ƒI[ƒi[‚ÌOnCollision‚ðŒÄ‚Ño‚·
	if (_owner)
	{
		_owner->OnCollision(other);
	}
}

Vector4 CapsuleCollider::GetCapsuleStart() const
{
	return _owner->GetPosition() + Vector4(0.0f, _halfHeight, 0.0f);
}

Vector4 CapsuleCollider::GetCapsuleEnd() const
{
	return _owner->GetPosition() + Vector4(0.0f, -_halfHeight, 0.0f);
}
