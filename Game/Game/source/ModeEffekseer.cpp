
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeEffekseer.h"

bool ModeEffekseer::Initialize() {
	if (!base::Initialize()) { return false; }

	// エフェクトハンドルの読み込み
	LoadResource("test", "res/Effekseer/Laser01.efkefc", 1.0f);

	return true;
}

bool ModeEffekseer::Terminate() {
	base::Terminate();

    // 全ての再生を停止
    for (auto& effect : _playingEffects) {
        StopEffekseer3DEffect(effect.playingHandle);
    }
    _playingEffects.clear();

    // 全てのハンドルを削除
    for (auto& pair : _mapEffectHanle) {
        DeleteEffekseerEffect(pair.second);
    }
    _mapEffectHanle.clear();

	return true;
}

bool ModeEffekseer::Process() {
    if (!base::Process()) { return false; }

    int key = ApplicationMain::GetInstance()->GetKey();
    int trg = ApplicationMain::GetInstance()->GetTrg();

	// 全エフェクトの更新
    auto it = _playingEffects.begin();
    while (it != _playingEffects.end()) {
        // 1. 再生が終了しているかチェック
        if (IsEffekseer3DEffectPlaying(it->playingHandle) != 0) {
            // まだ再生中なら更新
            it->vPos.x += it->speed;
            SetPosPlayingEffekseer3DEffect(it->playingHandle, it->vPos.x, it->vPos.y, it->vPos.z);
            ++it;
        }
        else {
            // 再生終了したものはリストから削除
            it = _playingEffects.erase(it);
        }
    }

    // Effekseerにより再生中のエフェクトを更新する。
    UpdateEffekseer3D();

    return true;
}

bool ModeEffekseer::Render() {
    if (!base::Render()) { return false; }

    // DXライブラリのカメラとEffekseerのカメラを同期する。
    Effekseer_Sync3DSetting();

    // Effekseerにより再生中のエフェクトを描画する。
    DrawEffekseer3D();

    return true;
}

void ModeEffekseer::CreateEffect(const std::string& name, VECTOR vPos) {
	if (_mapEffectHanle.count(name) > 0) {// nameが存在する場合
        EffectInstance instance;
		instance.playingHandle = PlayEffekseer3DEffect(_mapEffectHanle[name]);// エフェクト再生
		instance.vPos.x = vPos.x;// 座標設定
        instance.vPos.y = vPos.y;
        instance.vPos.z = vPos.z;
        instance.speed = 0.0f;

        _playingEffects.push_back(instance);
    }
}

void ModeEffekseer::LoadResource(const std::string& name, const char* path, float scale) {
	int resHandle = LoadEffekseerEffect(path, scale);// エフェクトハンドルの読み込み
    if (resHandle != -1) {
		_mapEffectHanle[name] = resHandle;// 名前とハンドルを紐づける
    }
}