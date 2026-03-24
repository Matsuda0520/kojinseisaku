#pragma once
#include "UIElement.h"

class UISlider : public UIElement
{
public:
	// ローカル座標、サイズ、値の範囲、初期値
	UISlider(float x, float y, float width, float height, float minVal, float maxVal, float initVal);
	virtual ~UISlider() override = default;

	void Process() override;
	void Render() override;

	float GetValue() const { return _value; }

private:
	float _width, _height;
	float _min, _max;
	float _value;
	bool _isDragging;// つまみをドラッグ中か

};

