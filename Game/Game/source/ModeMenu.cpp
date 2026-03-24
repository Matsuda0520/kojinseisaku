
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeMenu.h"
#include "ModeGame.h"
#include "DebugCamera.h"

bool ModeMenu::Initialize() {
	if (!base::Initialize()) { return false; }

	_curPos = 0;
	_curAnimCnt = 0;

	return true;
}

bool ModeMenu::Terminate() {
	base::Terminate();

	ClearMenuItems();

	_modeGame->SetUseDebugCamera(false);

	return true;
}

bool ModeMenu::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	_modeGame->SetUseDebugCamera(true);

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	// デバッグカメラ操作
	if (_debugCamera) {
		// アナログスティック対応
		DINPUT_JOYSTATE di;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &di);
		float lx, ly, rx, ry;	// 左右アナログスティックの座標
		float analogMin = 0.3f;	// アナログ閾値
		// Logicoolパッドの場合
		lx = (float)di.X / 1000.f; ly = (float)di.Y / 1000.f;
		rx = (float)di.Rx / 1000.f; ry = (float)di.Ry / 1000.f;

		DebugCamera::StickInput input;
		input.lx = lx; input.ly = ly;
		input.rx = rx; input.ry = ry;
		input.analogMin = analogMin;

		_debugCamera->Process(input);
		_debugCamera->SetUp();
	}

	// ESCキーでメニューを閉じる
	bool close = false;
	if (trg & PAD_INPUT_9) {
		close = true;
	}

	// 上下でカーソル移動をする
	if (trg & PAD_INPUT_UP) { _curPos--; _curAnimCnt = 0; }
	if (trg & PAD_INPUT_DOWN) { _curPos++; _curAnimCnt = 0; }

	// カーソル位置を上下ループ
	int itemNum = _menuItems.size();
	_curPos = (_curPos + itemNum) % itemNum;

	// 決定でアイテムのSelected()を呼ぶ
	if (trg & PAD_INPUT_1) {
		int ret = _menuItems[_curPos]->Selected();
		if (ret == 1) {
			// メニューを閉じる
			close = true;
		}
	}

	// メニューを閉じる
	if (close) {
		// このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}

	_curAnimCnt++;

	return true;
}

bool ModeMenu::Render() {
	base::Render();

	// メニュー項目の確認
	int x = 128, y = 128, w = 0, h = 0, fontSize = 32, fontPitch = fontSize + 8;
	SetFontSize(fontSize);
	for (auto ite = _menuItems.begin(); ite != _menuItems.end(); ite++) {
		int itemW = GetDrawStringWidth((*ite)->_text.c_str(), strlen((*ite)->_text.c_str()));
		if (w < itemW) {
			w = itemW;
		}
		h += fontPitch;
	}
	// カーソル, 枠分のサイズ拡張
	w += 64 + 16; h += 16;

	// 下地の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(x, y, x + w, y + h, GetColor(0, 0, 255), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(x, y, x + w, y + h, GetColor(0, 0, 255), TRUE);

	// メニュー項目の描画
	int startY = 16 / 2;
	h = 0;
	for (auto ite = _menuItems.begin(); ite != _menuItems.end(); ite++) {
		DrawString(x + 64, y + startY + h, (*ite)->_text.c_str(), GetColor(255, 0, 0));
		h += fontPitch;
	}

	// カーソルの描画
	// x座標はアニメーションする
	DrawGraph(x + 4 + ((_curAnimCnt / 6) % 4) * 4, y + startY + fontPitch * _curPos, gGlobal._cgCursor, TRUE);

	return true;
}

void ModeMenu::AddMenuItem(MenuItemBase* item) {
	_menuItems.push_back(std::unique_ptr<MenuItemBase>(item));
}

void ModeMenu::ClearMenuItems() {
	// 登録したアイテムをすべて削除する
	_menuItems.clear();
}