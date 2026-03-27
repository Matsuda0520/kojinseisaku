#include "ModeGameOver.h"
#include "ModeGame.h"
#include "ModeTitle.h"
#include "ActionMap.h"

bool ModeGameOver::Process()
{
	// 下位レイヤーのProcessを停止
	ModeServer::GetInstance().SkipProcessUnderLayer();

	if (ActionMap::GetInstance().IsTrigger(InputAction::Start))
	{
		ModeBase* game = ModeServer::GetInstance().Get("game");
		if (game)
		{
			ModeServer::GetInstance().Del(game);
		}

		ModeBase* title = ModeServer::GetInstance().Get("title");
		if (title)
		{
			ModeServer::GetInstance().Del(title);
		}

		auto newGame = std::make_unique<ModeGame>();
		ModeServer::GetInstance().Add(std::move(newGame), 1, "game");

		auto newTitle = std::make_unique<ModeTitle>();
		ModeServer::GetInstance().Add(std::move(newTitle), 50, "title");

		Destroy();
	}

	return true;
}

bool ModeGameOver::Render()
{
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);

	const int screenW = 1920;
	const int screenH = 1080;

	const char* gameOverText = "Game Over";
	const char* guideText = "Press Y Button To Title";

	// タイトル
	SetFontSize(96);
	const int gameOverW = GetDrawStringWidth(gameOverText, -1);
	const int gameOverX = (screenW - gameOverW) / 2;
	const int gameOverY = (screenH / 2) - 140;
	DrawString(gameOverX, gameOverY, gameOverText, GetColor(255, 255, 255));

	// ガイド
	SetFontSize(42);
	const int guideW = GetDrawStringWidth(guideText, -1);
	const int guideX = (screenW - guideW) / 2;
	const int guideY = (screenH / 2) + 10;
	DrawString(guideX, guideY, guideText, GetColor(255, 220, 120));

	return true;
}
