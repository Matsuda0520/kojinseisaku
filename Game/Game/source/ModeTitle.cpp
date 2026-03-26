#include "ModeTitle.h"
#include "ModeGame.h"
#include "ActionMap.h"

bool ModeTitle::Process()
{
	if (ActionMap::GetInstance().IsTrigger(InputAction::Start))
	{
		ModeBase* mode = ModeServer::GetInstance().Get("game");
		ModeGame* game = dynamic_cast<ModeGame*>(mode);
		if (game)
		{
			game->RequestStartMainGame();
		}
		Destroy();
	}

	return true;
}

bool ModeTitle::Render()
{
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);

	const int screenW = 1920;
	const int screenH = 1080;

	const char* titleText = "Dodge Laser";
	const char* guideText = "Press Enter To Start";

	// タイトル
	SetFontSize(96);
	const int titleW = GetDrawStringWidth(titleText, -1);
	const int titleX = (screenW - titleW) / 2;
	const int titleY = (screenH / 2) - 140;
	DrawString(titleX, titleY, titleText, GetColor(255, 255, 255));

	// ガイド
	SetFontSize(42);
	const int guideW = GetDrawStringWidth(guideText, -1);
	const int guideX = (screenW - guideW) / 2;
	const int guideY = (screenH / 2) + 10;
	DrawString(guideX, guideY, guideText, GetColor(255, 220, 120));

	return true;
}
