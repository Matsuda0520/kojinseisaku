#pragma once

// ダメージを与えるオブジェクトのインターフェース
class IAttacker
{
public:
	virtual ~IAttacker() = default;

	// 与えるダメージ量を返す
	virtual float GetAttackPower() const = 0;
};
