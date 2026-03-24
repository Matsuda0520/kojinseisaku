#include "CameraManager.h"
#include "CameraBase.h"

CameraManager::CameraManager(const char* name)
	: GameObject(name)
	, _activeCamera(nullptr)
{
	// カメラのNear,Farの距離を設定
	SetCameraNearFar(10.0f, 10000.0f);
}

void CameraManager::Process()
{
	if (_isDead) { return; }
	if (!_activeCamera) { return; }

	// アクティブカメラのProcessを呼び出す
	_activeCamera->Process();

	// DxLibのカメラ設定に適用する
	SetCameraPositionAndTarget_UpVecY(
		ToDX(_activeCamera->GetPosition()),// カメラの位置
		ToDX(_activeCamera->GetLookAt())// カメラの注視点
	);
}

void CameraManager::AddCamera(const std::string& name, std::unique_ptr<CameraBase> camera)
{
	if (!camera) { return; }

	// これが最初のカメラかチェック
	bool isFirst = _cameras.empty();

	CameraBase* camPtr = camera.get();

	// 所有権をマップに移動
	_cameras[name] = std::move(camera);

	// 最初に登録されたカメラをアクティブにする
	if (isFirst)
	{
		_activeCamera = camPtr;
	}
}

void CameraManager::SwitchCamera(const std::string& name)
{
	auto it = _cameras.find(name);
	// 指定された名前のカメラが存在する場合、アクティブカメラを切り替える
	if (it != _cameras.end())
	{
		_activeCamera = it->second.get();
	}
}
