#include "ModeGame.h"
#include "ModeGameOver.h"
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
#include "UIDamageFlash.h"
#include "UIScore.h"

ModeGame::ModeGame()
	: _cameraManager(nullptr)
	, _lightManager(nullptr)
	, _tpsCamera(nullptr)
	, _stageSpawner(nullptr)
	, _player(nullptr)
	, _shadowMapHandle(-1)
	, _isTitleDemo(true)
	, _isStartMainGameRequested(false)
	, _isGameOver(false)
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
	_stageSpawner = stageSpawner.get();
	// 初期ステージの構築
	_stageSpawner->BuildInitialStage();

	// ---------------------------------------------------------
	// プレイヤーの作成
	// ---------------------------------------------------------
	_playerSpawner = std::make_unique<Spawner>(_sceneRoot.get());
	GameObject* playerPtr = _playerSpawner->SpawnPlayer("Player", Vector4(0.0f, 0.0f, 0.0f));
	_player = playerPtr->AsPlayer();

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
	_stageSpawner->SetTarget(playerPtr);
	_sceneRoot->AddChild(std::move(stageSpawner));

	if (_stageSpawner)
	{
		_stageSpawner->SetLaserSpawnEnabled(false);
	}

	if (_tpsCamera)
	{
		_tpsCamera->SetTarget(playerPtr);
		_tpsCamera->SetOffset(-180.0f, 150.0f, 100.0f);// デモ用の近距離オフセット
	}

	// ---------------------------------------------------------
	// UIルートノードの作成
	// ---------------------------------------------------------
	_uiRoot = std::make_unique<UIPanel>(0.0f, 0.0f, 1920.0f, 1080.0f);// 解像度に合わせる
	// UI要素の生成と登録
	CharacterBase* character = playerPtr->AsCharacter();
	if (character)
	{
		character->AddObserver(this);

		auto hpBar = std::make_unique<UIHPBar>(character, 100.0f, 50.0f, 300.0f, 20.0f);
		_uiRoot->AddChild(std::move(hpBar));

		auto damageFlash = std::make_unique<UIDamageFlash>(character, 1920, 1080);// 解像度
		_uiRoot->AddChild(std::move(damageFlash));

		auto uiScore = std::make_unique<UIScore>(this, 1500.0f, 50.0f);
		_uiRoot->AddChild(std::move(uiScore));
	}

	_shadowMapHandle = MakeShadowMap(2048, 2048);

	return true;
}

bool ModeGame::Terminate()
{
	if (_uiRoot)
	{
		_uiRoot->Terminate();
		_uiRoot.reset();
	}

	if (_sceneRoot)
	{
		_sceneRoot->Terminate();
		_sceneRoot.reset();
	}

	_playerSpawner.reset();
	CollisionManager::GetInstance().Terminate();

	_scoreObservers.clear();

	return true;
}

bool ModeGame::Process()
{
	if (_isStartMainGameRequested)
	{
		StartMainGame();
		_isStartMainGameRequested = false;
	}

	// シーンの更新
	if (_sceneRoot)
	{
		// ルートツリーの更新
		_sceneRoot->Process();
	}

	if (!_isTitleDemo)
	{
		CollisionManager::GetInstance().Process();

		// UIの更新
		if (_uiRoot)
		{
			_uiRoot->Process();
		}
	}

	if (!_isTitleDemo && !_isGameOver)
	{
		// プレイヤーのスピード倍率を取得(速いほどスコアが増える)
		float speedMultiplier = 1.0f;
		if (_player) 
		{
			speedMultiplier = _player->GetSpeedMultiplier();
		}

		// 毎フレーム10点*スピード倍率
		_scoreAccumulator += 10.0f * speedMultiplier;

		// 浮動小数点で蓄積し、1以上になったら整数スコアに反映
		if (_scoreAccumulator >= 1.0f)
		{
			int addVal = static_cast<int>(_scoreAccumulator);
			AddScore(addVal);
			_scoreAccumulator -= addVal;// 端数を残す
		}
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
	if (!_isTitleDemo && _uiRoot)
	{
		_uiRoot->Render();
	}

	if (_isGameOver)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	return true;
}

void ModeGame::RequestStartMainGame()
{
	_isStartMainGameRequested = true;
}

void ModeGame::StartMainGame()
{
	if (!_isTitleDemo) { return; }

	_isTitleDemo = false;

	if (_player)
	{
		_player->SetSpeedUpActive(true);
	}

	if (_tpsCamera)
	{
		_tpsCamera->SetOffset(-450.0f, 400.0f, 150.0f);// 本編カメラへ戻す
	}

	if (_stageSpawner)
	{
		_stageSpawner->SetLaserSpawnEnabled(true);
	}
}

void ModeGame::OnDied()
{
	// プレイヤーが死亡したとき
	if (_isGameOver) { return; }

	_isGameOver = true;

	auto gameOver = std::make_unique<ModeGameOver>();
	ModeServer::GetInstance().Add(std::move(gameOver), 100, "gameover");
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

// スコア加算と通知処理
void ModeGame::AddScore(int amount)
{
	_score += amount;
	NotifyScoreChanged();
}

// Observer関連の実装
void ModeGame::AddScoreObserver(IScoreObserver* observer)
{
	if (std::find(
		_scoreObservers.begin(), _scoreObservers.end(), observer) == _scoreObservers.end()
		) 
	{
		_scoreObservers.push_back(observer);
		// 登録された瞬間に現在のスコアを教える
		observer->OnScoreChanged(_score);
	}
}

void ModeGame::RemoveScoreObserver(IScoreObserver* observer)
{
	_scoreObservers.erase(
		std::remove(_scoreObservers.begin(), _scoreObservers.end(), observer),
		_scoreObservers.end()
	);
}

void ModeGame::NotifyScoreChanged()
{
	for (auto* obs : _scoreObservers) 
	{
		if (obs) { obs->OnScoreChanged(_score); }
	}
}
