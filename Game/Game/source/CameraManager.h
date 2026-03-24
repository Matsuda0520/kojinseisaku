#pragma once
#include "GameObject.h"

class CameraBase;

class CameraManager : public GameObject
{
public:
	CameraManager(const char* name = "CameraManager");
	~CameraManager() override = default;

	void Initialize() override {}
	void Process() override;
	void Render() override {}
	void Terminate() override {}

	// カメラを名前付きで登録
	void AddCamera(const std::string& name, std::unique_ptr<CameraBase> camera);

	// 使用するカメラを切り替える
	void SwitchCamera(const std::string& name);

private:
	// カメラの種類ごとに管理
	std::unordered_map<std::string, std::unique_ptr<CameraBase>> _cameras;
	CameraBase* _activeCamera;// 現在アクティブなカメラへの参照
};

