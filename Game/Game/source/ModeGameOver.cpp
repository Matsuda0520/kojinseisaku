
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGameOver.h"
#include "ModeGame.h"

bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }

	// フォントの作成
	// 作成するフォントの名前、サイズ、太さ、タイプ
	_fontHandle = CreateFontToHandle(NULL, 64, 9, DX_FONTTYPE_NORMAL);

	return true;
}

bool ModeGameOver::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGameOver::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	if (trg & PAD_INPUT_4) {
		// ModeServerから"game"という名のModeGameを取得
		ModeGame* modeGame = dynamic_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));

		if (modeGame) {
			modeGame->Reset();// ゲーム状態の初期化
		}

		// このモードを削除する。ゲームの再スタート
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModeGameOver::Render() {
	base::Render();

	// このモードより下のレイヤーはRender()を呼ばない
	//ModeServer::GetInstance()->SkipRenderUnderLayer();

	// 半透明の黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawFillBox(0, 0, 1280, 720, GetColor(0, 0, 0));// 中身を塗りつぶす四角形を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フォントハンドルを使って文字列を描画
	DrawStringToHandle(300, 200, "GAME OVER!!!", GetColor(255, 0, 0), _fontHandle);

	// 最終スコア表示
	char scoreStr[64];
	sprintf_s(scoreStr, "SCORE: %d", _resultScore);
	DrawStringToHandle(300, 300, scoreStr, GetColor(255, 255, 255), _fontHandle);

	return true;
}

