#pragma once
#include "MenuItemBase.h"

class ModeGame;

class MenuItemViewCollision : public MenuItemBase {
public:
	MenuItemViewCollision(void* param, std::string text) : MenuItemBase(param, text) {}

	// 項目を決定したらこの関数が呼ばれる
	// return int : 0 = メニュー継続, 1 = メニュー終了
	virtual int Selected() override;
};

class MenuItemDebugCamera : public MenuItemBase {
	public:
	MenuItemDebugCamera(void* param, std::string text) : MenuItemBase(param, text) {}
	// 項目を決定したらこの関数が呼ばれる
	// return int : 0 = メニュー継続, 1 = メニュー終了
	virtual int Selected() override;
};

class MenuItemUseCollision : public MenuItemBase {
	public:
	MenuItemUseCollision(void* param, std::string text) : MenuItemBase(param, text) {}
	// 項目を決定したらこの関数が呼ばれる
	// return int : 0 = メニュー継続, 1 = メニュー終了
	virtual int Selected() override;
};

