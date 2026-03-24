#pragma once

class UIElement;

// UI関連のイベントを受け取るためのインターフェース
class IUIObserver
{
public:
	virtual ~IUIObserver() = default;

	// イベントごとの実装
	// UIがクリックされたとき
	virtual void OnUIClicked(UIElement* element) = 0;
	// マウスがUIに入ったとき　離れたとき
	virtual void OnUIHoverEntered(UIElement* element) = 0;
	virtual void OnUIHoverExited(UIElement* element) = 0;

	// スライダーの値が変更されたとき
	virtual void OnUIValueChanged(UIElement* element, float value) = 0;
};
