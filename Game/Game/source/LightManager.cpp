#include "LightManager.h"

LightManager::LightManager(const char* name)
	: GameObject(name)
{
	// ライト計算を有効にする
	SetUseLighting(TRUE);

	// DxLibの標準ライトを有効化
	SetLightEnable(TRUE);
}

LightManager::~LightManager()
{
	// 生成したライトをすべて削除
	for (int handle : _lightHandles)
	{
		DeleteLightHandle(handle);
	}
	_lightHandles.clear();
}

void LightManager::Process()
{
	if (_isDead) { return; }
}

void LightManager::SetAmbientLight(const Vector4& color)
{
	SetGlobalAmbientLight(GetColorF(color.GetX(), color.GetY(), color.GetZ(), 1.0f));
}

void LightManager::SetMainLight(const Vector4& direction, const Vector4& color)
{
	// 標準ライトをディレクショナルライトとして設定
	ChangeLightTypeDir(ToDX(direction));

	// ライトの色をセット
	SetLightDifColor(GetColorF(color.GetX(), color.GetY(), color.GetZ(), 1.0f));
}

int LightManager::AddPointLight(const Vector4& position, float range, const Vector4& color)
{
	// ポイントライトを生成
	// 位置、影響範囲、減衰パラメータ
	int handle = CreatePointLightHandle(
		ToDX(position),
		range,
		0.0f,
		0.002f,
		0.0f
	);

	if (handle >= 0)
	{
		SetLightDifColorHandle(handle, (GetColorF(color.GetX(), color.GetY(), color.GetZ(), 1.0f)));
		_lightHandles.push_back(handle);
	}

	return handle;
}

void LightManager::RemoveLight(int lightHandle)
{
	auto it = std::find(_lightHandles.begin(), _lightHandles.end(), lightHandle);

	if (it != _lightHandles.end())
	{
		DeleteLightHandle(*it);
		_lightHandles.erase(it);
	}
}
