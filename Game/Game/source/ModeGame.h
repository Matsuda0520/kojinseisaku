#pragma once
#include "appframe.h"
#include "IUIObserver.h"
#include "IHPObserver.h"
#include "IScoreObserver.h"

class GameComposite;
class Spawner;
class StageSpawner;
class CameraManager;
class LightManager;
class TPSCamera;
class UIPanel;
class Player;

class ModeGame : public ModeBase, public IUIObserver, public IHPObserver
{
public:
	ModeGame();
	virtual ~ModeGame();

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

	void RequestStartMainGame();

	// 通知を受け取ったときの処理
	// プレイヤー関連
	void OnDied() override;

	// UI関連
	void OnUIClicked(UIElement* sender) override;
	void OnUIHoverEntered(UIElement* sender) override;
	void OnUIHoverExited(UIElement* sender) override;
	void OnUIValueChanged(UIElement* sender, float value) override;

	void AddScoreObserver(IScoreObserver* observer);
	void RemoveScoreObserver(IScoreObserver* observer);

private:
	void StartMainGame();
	void AddScore(int amount);
	void NotifyScoreChanged();

	std::unique_ptr<GameComposite> _sceneRoot;// シーンのルートノード
	std::unique_ptr<UIPanel> _uiRoot;// UIのルートノード
	std::unique_ptr<Spawner> _playerSpawner;// プレイヤースポナー

	CameraManager* _cameraManager;
	LightManager* _lightManager;
	TPSCamera* _tpsCamera;
	StageSpawner* _stageSpawner;
	Player* _player;

	int _shadowMapHandle;// シャドウマップのハンドル

	bool _isTitleDemo;
	bool _isStartMainGameRequested;

	bool _isGameOver;

	int _score = 0;
	float _scoreAccumulator = 0.0f;
	std::vector<IScoreObserver*> _scoreObservers;
};

