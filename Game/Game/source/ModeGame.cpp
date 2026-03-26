#include "ModeGame.h"
#include "GameComposite.h"
#include "CharacterBase.h"
#include "GameObjectFactory.h"
#include "Spawner.h"
#include "StageSpawner.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "Player.h"
#include "TPSCamera.h"
#include "UIPanel.h"
#include "UIHPBar.h"

ModeGame::ModeGame()
	: _cameraManager(nullptr)
	, _lightManager(nullptr)
	, _tpsCamera(nullptr)
	, _shadowMapHandle(-1)
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

	// ---------------------------------------------------------
	// UIルートノードの作成
	// ---------------------------------------------------------
	_uiRoot = std::make_unique<UIPanel>(0.0f, 0.0f, 1280.0f, 720.0f);// 解像度に合わせる
	// UI要素の生成と登録
	CharacterBase* character = playerPtr->AsCharacter();
	if (character)
	{
		auto hpBar = std::make_unique<UIHPBar>(character, 50.0f, 50.0f, 300.0f, 20.0f);
		_uiRoot->AddChild(std::move(hpBar));
	}

	_shadowMapHandle = MakeShadowMap(2048, 2048);

	return true;
}

bool ModeGame::Terminate()
{
	if (_sceneRoot)
	{
		_sceneRoot->Terminate();
		_sceneRoot.reset();
	}

	_playerSpawner.reset();
	CollisionManager::GetInstance().Terminate();

	return true;
}

bool ModeGame::Process()
{
	// シーンの更新
	if (_sceneRoot)
	{
		// ルートツリーの更新
		_sceneRoot->Process();
	}
	CollisionManager::GetInstance().Process();

	// UIの更新
	if (_uiRoot)
	{
		_uiRoot->Process();
	}

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

	//----------------------------------------------------------------------------
	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(_shadowMapHandle, VGet(0.0f, -1.0f, -0.5f));// 真下からの光を想定

	// シャドウマップに描画する範囲を設定
	// カメラの注視点を中心にする
	float length = 1000.0f;
	Vector4 minPos = _tpsCamera->GetLookAt() - Vector4(length, length, length);
	Vector4 maxPos = _tpsCamera->GetLookAt() + Vector4(length, length, length);
	SetShadowMapDrawArea(_shadowMapHandle, ToDX(minPos), ToDX(maxPos));

	// 2回まわして、path=0:シャドウマップへの描画, path=1:モデルの描画
	for (int path = 0; path < 2; path++)
	{
		if (path == 0)
		{
			// シャドウマップへの描画の準備
			ShadowMap_DrawSetup(_shadowMapHandle);

			if (_sceneRoot)
			{
				// ルートツリーの描画
				_sceneRoot->Render();
			}
		}
		else if (path == 1)
		{
			// シャドウマップへの描画を終了
			ShadowMap_DrawEnd();
			// 描画に使用するシャドウマップを設定
			SetUseShadowMap(0, _shadowMapHandle);

			if (_sceneRoot)
			{
				// ルートツリーの描画
				_sceneRoot->Render();
			}
		}
	}

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	//----------------------------------------------------------------------------

	//CollisionManager::GetInstance().Render();

	// UIの描画
	SetUseZBuffer3D(FALSE);// Zバッファを無効にする
	SetWriteZBuffer3D(FALSE);
	if (_uiRoot)
	{
		_uiRoot->Render();
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
