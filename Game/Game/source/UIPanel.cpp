#include "UIPanel.h"

void UIPanel::Process()
{
	if (_isDead) { return; }

	// 追加予約された子をコンテナに移す
	for (auto& newChild : _pendingChildren)
	{
		_children.push_back(std::move(newChild));
	}
	_pendingChildren.clear();

	// 破棄された要素の削除
	_children.erase(
		std::remove_if(_children.begin(), _children.end(),
			[](const std::unique_ptr<UIElement>& child)
			{
				return child->IsDead();
			}),
		_children.end());

	// 子のProcessを呼び出す
	for (auto& child : _children)
	{
		child->Process();
	}
}

void UIPanel::Render()
{
	if (_isDead) { return; }

	// まずパネルを描画する
	Vector2 globalPos = GetGlobalPos();
	int gx = static_cast<int>(globalPos.GetX());
	int gy = static_cast<int>(globalPos.GetY());

	// その上に子要素を描画する
	for (auto& child : _children)
	{
		child->Render();
	}

	// 仮実装: 右下に操作ガイドを描画
	const char* jumpText = "A Button : Jump";
	const char* rollText = "B Button : Roll";

	const int marginRight = 24;
	const int marginBottom = 24;
	const int lineHeight = 28;
	const int lineSpacing = 34;

	SetFontSize(28);

	const int panelRight = gx + static_cast<int>(_width);
	const int panelBottom = gy + static_cast<int>(_height);

	const int jumpW = GetDrawStringWidth(jumpText, -1);
	const int rollW = GetDrawStringWidth(rollText, -1);

	const int jumpX = panelRight - marginRight - jumpW;
	const int rollX = panelRight - marginRight - rollW;

	const int rollY = panelBottom - marginBottom - lineHeight;
	const int jumpY = rollY - lineSpacing;

	DrawString(jumpX, jumpY, jumpText, GetColor(255, 255, 255));
	DrawString(rollX, rollY, rollText, GetColor(255, 255, 255));
}

void UIPanel::AddChild(std::unique_ptr<UIElement> child)
{
	if (child)
	{
		child->SetParent(this);// 自身を親として設定する
		_pendingChildren.push_back(std::move(child));// 追加予約する
	}
}
