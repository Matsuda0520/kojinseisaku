
#include "appframe.h"
#include "MenuItemBase.h"

#include <vector>
#include <memory>

class ModeGame;
class DebugCamera;

class ModeMenu : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void AddMenuItem(MenuItemBase* item);
	void ClearMenuItems();

	void SetModeGame(ModeGame* modeGame) { _modeGame = modeGame; }
	void SetDebugCamera(std::unique_ptr<DebugCamera>& cam) { _debugCamera = cam.get(); }// èäóLÇÕÇπÇ∏éQè∆ÇÃÇ›


protected:
	//std::vector<MenuItemBase*> _menuItems;
	std::vector<std::unique_ptr<MenuItemBase>> _menuItems;

	int _curPos;
	int _curAnimCnt;

	ModeGame* _modeGame = nullptr;
	DebugCamera* _debugCamera = nullptr;
};

