#include "ModeGame.h"
#include "GameComposite.h"
#include "GameObjectFactory.h"
#include "Spawner.h"
#include "StageSpawner.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "Player.h"
#include "TPSCamera.h"

ModeGame::ModeGame()
	: _cameraManager(nullptr)
	, _lightManager(nullptr)
	, _tpsCamera(nullptr)
{
}

ModeGame::~ModeGame()
{
}

bool ModeGame::Initialize()
{
	// ---------------------------------------------------------
	// ファクトリーへのクラス登録
	// ---------------------------------------------------------
	auto& factory = GameObjectFactory::GetInstance();
	factory.RegisterType<Player>("Player");

	// ---------------------------------------------------------
	// ルートノードの作成
	// ---------------------------------------------------------
	_sceneRoot = std::make_unique<GameComposite>("SceneRoot");

	// ---------------------------------------------------------
	// カメラ・ライトマネージャーの作成
	// ---------------------------------------------------------
	auto camManager = std::make_unique<CameraManager>();
	_cameraManager = camManager.get();
	_sceneRoot->AddChild(std::move(camManager));

	auto lightManager = std::make_unique<LightManager>();
	_lightManager = lightManager.get();
	_sceneRoot->AddChild(std::move(lightManager));

	// ---------------------------------------------------------
	// ステージの作成
	// ---------------------------------------------------------
	auto stageSpawner = std::make_unique<StageSpawner>();
	// 初期ステージの構築
	stageSpawner->BuildInitialStage();

	// ---------------------------------------------------------
	// プレイヤーの作成
	// ---------------------------------------------------------
	_playerSpawner = std::make_unique<Spawner>(_sceneRoot.get());
	GameObject* playerPtr = _playerSpawner->SpawnPlayer("Player", Vector4(0.0f, 0.0f, 0.0f));

	// ---------------------------------------------------------
	// カメラの作成と登録
	// ---------------------------------------------------------
	auto tpsCamera = std::make_unique<TPSCamera>();
	_tpsCamera = tpsCamera.get();
	// 所有権をカメラマネージャーに渡す
	_cameraManager->AddCamera("TPS", std::move(tpsCamera));

	// ---------------------------------------------------------
	// プレイヤーの登録
	// ---------------------------------------------------------
	stageSpawner->SetTarget(playerPtr);
	_sceneRoot->AddChild(std::move(stageSpawner));

	if (_tpsCamera)
	{
		_tpsCamera->SetTarget(playerPtr);
	}

	return true;
}

bool ModeGame::Terminate()
{
	return true;
}

bool ModeGame::Process()
{
	if (_sceneRoot)
	{
		// ルートツリーの更新
		_sceneRoot->Process();
	}

	CollisionManager::GetInstance().Process();

	return true;
}

bool ModeGame::Render()
{
	// 3D基本設定
	{
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);
		SetUseBackCulling(TRUE);
	}

	if (_sceneRoot)
	{
		// ルートツリーの描画
		_sceneRoot->Render();
	}

	return true;
}

void ModeGame::OnPlayerDamaged(Player* player)
{
	// プレイヤーがダメージを受けたとき
}

void ModeGame::OnPlayerDied(Player* player)
{
	// プレイヤーが死亡したとき
}

void ModeGame::OnUIClicked(UIElement* sender)
{
	// UIがクリックされたとき
}

void ModeGame::OnUIHoverEntered(UIElement* sender)
{
	// UIにホバーが入ったとき
}

void ModeGame::OnUIHoverExited(UIElement* sender)
{
	// UIからホバーが外れたとき
}

void ModeGame::OnUIValueChanged(UIElement* sender, float value)
{
	// UIの値が変更されたとき
}
