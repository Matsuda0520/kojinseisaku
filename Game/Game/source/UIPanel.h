#pragma once
#include "UIElement.h"

// 他のUIをまとめるパネルクラス
class UIPanel : public UIElement
{
public:
	UIPanel(float x, float y, float width, float height)
		: UIElement(x, y), _width(width), _height(height) {}
	UIPanel(const Vector2& pos, float width, float height)
		: UIElement(pos.GetX(), pos.GetY()), _width(width), _height(height) {}

	void Process() override;
	void Render() override;

	void AddChild(std::unique_ptr<UIElement> child) override;

private:
	float _width, _height;// パネルのサイズ
	std::vector<std::unique_ptr<UIElement>> _children;
	// 追加予約用コンテナ。次のフレームで_childrenに移す
	std::vector<std::unique_ptr<UIElement>> _pendingChildren;
};

