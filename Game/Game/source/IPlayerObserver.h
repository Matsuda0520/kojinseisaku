#pragma once

class Player;

// プレイヤー関連のイベントを受け取るためのインターフェース
class IPlayerObserver
{
public:
	virtual ~IPlayerObserver() = default;

	virtual void OnPlayerDamaged(Player* player) = 0;
	virtual void OnPlayerDied(Player* player) = 0;

};

