#pragma once
#include "appframe.h"

using Math::Vector2;

class IUIObserver;

// UI要素の基底クラス
class UIElement
{
public:
	UIElement(float x, float y) 
		: _localPos(x, y), _parent(nullptr), _isDead(false) {}
	virtual ~UIElement() = default;

	virtual void Initialize() = 0;
	virtual void Process() = 0;
	virtual void Render() = 0;
	virtual void Terminate() = 0;
	
	// 破棄する
	virtual void Destroy() { _isDead = true; }

	// 子要素を追加する
	virtual void AddChild(std::unique_ptr<UIElement> child) {}

	// 座標管理
	Vector2 GetLocalPos() const { return _localPos; }
	void SetLocalPos(const Vector2& pos) { _localPos = pos; }
	void SetLocalPos(float x, float y) { _localPos.Set(x, y); }

	// 親を辿ってグローバル座標を計算する
	Vector2 GetGlobalPos() const;

	bool IsDead() { return _isDead; } const

	void SetParent(UIElement* parent) { _parent = parent; }

	// Observer管理
	void AddObserver(IUIObserver* observer);
	void RemoveObserver(IUIObserver* observer);

protected:
	// イベントごとの通知関数
	void NotifyClicked();
	void NotifyHoverEntered();
	void NotifyHoverExited();
	void NotifyValueChanged(float value);

	Vector2 _localPos;// ローカル座標
	UIElement* _parent;// 親への参照
	bool _isDead;

private:
	std::vector<IUIObserver*> _observers;// 登録されたオブザーバーのリスト

};

