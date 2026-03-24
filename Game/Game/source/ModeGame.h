#pragma once
#include "appframe.h"
#include "IPlayerObserver.h"
#include "IUIObserver.h"

class GameComposite;
class Spawner;
class StageSpawner;
class CameraManager;
class LightManager;
class TPSCamera;

class ModeGame : public ModeBase, public IPlayerObserver, public IUIObserver
{
public:
	ModeGame();
	virtual ~ModeGame();

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

	// 通知を受け取ったときの処理
	// プレイヤー関連
	void OnPlayerDamaged(Player* player) override;
	void OnPlayerDied(Player* player) override;
	// UI関連
	void OnUIClicked(UIElement* sender) override;
	void OnUIHoverEntered(UIElement* sender) override;
	void OnUIHoverExited(UIElement* sender) override;
	void OnUIValueChanged(UIElement* sender, float value) override;

private:
	std::unique_ptr<GameComposite> _sceneRoot;// シーンのルートノード
	std::unique_ptr<Spawner> _playerSpawner;// プレイヤースポナー

	CameraManager* _cameraManager;
	LightManager* _lightManager;
	TPSCamera* _tpsCamera;
};

