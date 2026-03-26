#pragma once

// Hpが変化したときに通知を受け取る汎用インターフェース
class IHPObserver
{
public:
	virtual ~IHPObserver() = default;

	// Hpが変化したとき、現在のHpと最大Hpを渡す
	virtual void OnHPChanged(float currentHp, float maxHp) {}
	virtual void OnDied() {}
	// 対象が破棄されたときの通知
	virtual void OnTargetDestroyed() {}
};
