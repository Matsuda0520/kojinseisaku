#pragma once
#include "appframe.h"

#include <string>
#include <vector>
#include <map>

// 再生中の個別エフェクトを管理する構造体
struct EffectInstance {
	int playingHandle;// 再生中ハンドル
	VECTOR vPos;// 座標
	float speed;// 移動速度
};

class ModeEffekseer : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	// エフェクトを作成する関数
	void CreateEffect(const std::string& name, VECTOR vPos);

protected:
	// エフェックトハンドルの管理用
	// 初期化時に読み込んで、名前とハンドルを紐づける
	std::map<std::string, int> _mapEffectHanle;

	std::vector<EffectInstance> _playingEffects;// 再生中エフェクトリスト

	// エフェクトハンドルの読み込み
	void LoadResource(const std::string& name, const char* path, float scale);
};

