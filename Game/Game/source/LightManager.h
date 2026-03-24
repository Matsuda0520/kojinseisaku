#pragma once
#include "GameObject.h"

// ライトの管理を行うクラス
class LightManager : public GameObject
{
public:
	LightManager(const char* name = "LightManager");
	~LightManager() override;

	void Initialize() override {};
	void Process() override;
	void Render() override {}
	void Terminate() override {};

	// 環境光を設定
	void SetAmbientLight(const Vector4& color);

	// ディレクショナルライトの方向、色を設定
	void SetMainLight(const Vector4& direction, const Vector4& color);

	// ポイントライトを追加。ハンドルを返す
	int AddPointLight(const Vector4& position, float range, const Vector4& color);

	// 特定のライトを削除
	void RemoveLight(int lightHandle);

private:
	std::vector<int> _lightHandles;// ライトのハンドルリスト

};

