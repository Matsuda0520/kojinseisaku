#pragma once

class GameObject;

// 発生するイベントの種類
enum class GameEvent
{
	_EOT_
};

// Observerインターフェース
class IObserver
{
public:
	virtual ~IObserver() = default;

	// イベントが通知されたときに呼ばれる関数
	virtual void OnNotify(GameObject* sender, GameEvent event) = 0;
};

