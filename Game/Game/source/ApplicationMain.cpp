#include "ApplicationMain.h"
#include "ModeGame.h"

// グローバルなインスタンス
ApplicationMain g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance)
{
	// 基底クラスの初期化
	if (!base::Initialize(hInstance)) { return false; }

	// モードの追加
	auto game = std::make_unique<ModeGame>();
	ModeServer::GetInstance().Add(std::move(game), 1, "game");

	return true;
}

bool ApplicationMain::Terminate()
{
	ModeServer::GetInstance().Clear();

	// 最後に基底クラスの終了処理
	base::Terminate();

	return true;
}

bool ApplicationMain::Input()
{
	base::Input();

	InputManager::GetInstance().Update();

	return true;
}

bool ApplicationMain::Process()
{
	base::Process();

	return true;
}

bool ApplicationMain::Render()
{
	base::Render();

	return true;
}