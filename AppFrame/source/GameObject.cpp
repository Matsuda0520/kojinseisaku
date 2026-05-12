#include "GameObject.h"

GameObject::GameObject(const char* name)
	: _name(name)
	, _isDead(false)
	, _parent(nullptr)
	, _position(0.0f, 0.0f, 0.0f)
	, _rotation(0.0f, 0.0f, 0.0f)
	, _scale(1.0f, 1.0f, 1.0f)
{
}

void GameObject::Initialize()
{
	for (auto& component : _components)
	{
		component->Initialize();
	}
}

void GameObject::Terminate()
{
	for (auto& component : _components)
	{
		component->Terminate();
	}
}

void GameObject::Process()
{
	if (_isDead) { return; }

	for (auto& component : _components)
	{
		component->Process();
	}
}

void GameObject::Render()
{
	if (_isDead) { return; }

	for (auto& component : _components)
	{
		component->Render();
	}
}