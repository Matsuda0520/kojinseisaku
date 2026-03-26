#pragma once

class UIElement;

// UI関連のイベントを受け取るためのインターフェース
class IUIObserver
{
public:
	virtual ~IUIObserver() = default;

	// イベントごとの実装
	// UIがクリックされたとき
	virtual void OnUIClicked(UIElement* element) {}
	// マウスがUIに入ったとき　離れたとき
	virtual void OnUIHoverEntered(UIElement* element) {}
	virtual void OnUIHoverExited(UIElement* element) {}

	// スライダーの値が変更されたとき
	virtual void OnUIValueChanged(UIElement* element, float value) {}
};
