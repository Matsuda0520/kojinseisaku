#include "DebugUtility.h"
#include "ModeGame.h"

int MenuItemViewCollision::Selected() {
    ModeGame* mdGame = static_cast<ModeGame*>(_param);
    mdGame->SetDebugViewCollision(!mdGame->GetDebugViewCollision());
    return 1;
}

int MenuItemDebugCamera::Selected() {
    ModeGame* mdGame = static_cast<ModeGame*>(_param);
    mdGame->SetUseDebugCamera(true);
    return 0;
}

int MenuItemUseCollision::Selected() {
    ModeGame* mdGame = static_cast<ModeGame*>(_param);
    return 1;
}
