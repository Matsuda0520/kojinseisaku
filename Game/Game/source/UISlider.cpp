#include "UISlider.h"

UISlider::UISlider(float x, float y, float width, float height, float minVal, float maxVal, float initVal)
	: UIElement(x, y)
	, _width(width)
	, _height(height)
	, _min(minVal)
	, _max(maxVal)
	, _value(initVal)
	, _isDragging(false)
{
}

void UISlider::Process()
{
	if (_isDead) { return; }

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int mouseInput = GetMouseInput();

	Vector2 globalPos = GetGlobalPos();
	float gx = globalPos.GetX();
	float gy = globalPos.GetY();

	// 当たり判定
	bool isHovered = (mouseX >= gx && mouseX <= gx + _width &&
		mouseY >= gy && mouseY <= gy + _height);

	// スライダー上でクリックされたらドラッグ開始
	if (isHovered && (mouseInput & MOUSE_INPUT_LEFT))
	{
		_isDragging = true;
	}

	// 左クリックが離されたらドラッグ終了
	if(!(mouseInput & MOUSE_INPUT_LEFT))
	{
		_isDragging = false;
	}

	// ドラッグ中の処理
	if (_isDragging)
	{
		// スライダーの左端を0.0, 右端を1.0としたときの割合を計算
		float ratio = (mouseX - gx) / _width;

		// 範囲内にクランプ
		ratio = Math::Clamp01(ratio);

		// 割合を実際の値に変換
		float newValue = Math::Lerp(_min, _max, ratio);

		// 値が変わったら通知
		if (_value != newValue)
		{
			_value = newValue;
			NotifyValueChanged(_value);
		}
	}
}

void UISlider::Render()
{
	if (_isDead) { return; }

	Vector2 globalPos = GetGlobalPos();
	int gx = static_cast<int>(globalPos.GetX());
	int gy = static_cast<int>(globalPos.GetY());

	// 背景のバーを描画
	DrawBox(gx, gy, gx + static_cast<int>(_width), gy + static_cast<int>(_height), GetColor(80, 80, 80), TRUE);
	DrawBox(gx, gy, gx + static_cast<int>(_width), gy + static_cast<int>(_height), GetColor(255, 255, 255), FALSE);

	// 現在の値から、つまみのX座標を計算
	float ratio = (_value - _min) / (_max - _min);
	int knobX = gx + static_cast<int>(_width * ratio);
	int knobWidth = 10;// つまみの幅

	// つまみを描画
	// バーより少し上下にはみ出すように描画する
	DrawBox(knobX - knobWidth / 2, gy - 4, knobX + knobWidth / 2, gy + static_cast<int>(_height) + 4, GetColor(200, 200, 200), TRUE);
}