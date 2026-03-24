
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"

bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }

	// フォントの作成
	// 作成するフォントの名前、サイズ、太さ、タイプ
	_fontHandle = CreateFontToHandle(NULL, 64, 9, DX_FONTTYPE_NORMAL);

	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();

	// フォントの削除
	DeleteFontToHandle(_fontHandle);

	return true;
}

bool ModeTitle::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	if (trg & PAD_INPUT_4) {
		// ゲーム開始。このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModeTitle::Render() {
	base::Render();

	// このモードより下のレイヤーはRender()を呼ばない
	//ModeServer::GetInstance()->SkipRenderUnderLayer();

	// 半透明の黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawFillBox(0, 0, 1280, 720, GetColor(0, 0, 0));// 中身を塗りつぶす四角形を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フォントハンドルを使って文字列を描画
	DrawStringToHandle(300, 200, "GAME START!!!", GetColor(255, 255, 0), _fontHandle);

	return true;
}