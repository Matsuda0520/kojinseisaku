#include "UIElement.h"
#include "IUIObserver.h"

Vector2 UIElement::GetGlobalPos() const
{
	if (_parent)
	{
		// 自身のローカル座標 + 親のグローバル座標
		return _localPos + _parent->GetGlobalPos();
	}
	return _localPos;
}

void UIElement::AddObserver(IUIObserver* observer)
{
	_observers.push_back(observer);
}

void UIElement::RemoveObserver(IUIObserver* observer)
{
	_observers.erase(
		std::remove(
			_observers.begin(), _observers.end(), observer)
		, _observers.end());
}

void UIElement::NotifyClicked()
{
	for (auto* observer : _observers)
	{
		observer->OnUIClicked(this);
	}
}

void UIElement::NotifyHoverEntered()
{
	for (auto* observer : _observers)
	{
		observer->OnUIHoverEntered(this);
	}
}

void UIElement::NotifyHoverExited()
{
	for (auto* observer : _observers)
	{
		observer->OnUIHoverExited(this);
	}
}

void UIElement::NotifyValueChanged(float value)
{
	for (auto* observer : _observers)
	{
		observer->OnUIValueChanged(this, value);
	}
}