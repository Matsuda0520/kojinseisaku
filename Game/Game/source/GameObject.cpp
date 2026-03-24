#include "GameObject.h"

GameObject::GameObject(const char* name)
	: _name(name)
	, _isDead(false)
	, _parent(nullptr)
	, _pos(0.0f, 0.0f, 0.0f)
	, _rot(0.0f, 0.0f, 0.0f)
	, _scale(1.0f, 1.0f, 1.0f)
{
}

void GameObject::Destroy()
{
	// Ž€–Sƒtƒ‰ƒO‚ð—§‚Ä‚é
	if (!_isDead)
	{
		_isDead = true;
	}
}
