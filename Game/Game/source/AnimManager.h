#pragma once
#include "appframe.h"

// アニメーション管理クラス
// モデルに対して、アニメーションの再生、ブレンドのみを管理する
class AnimManager
{
public:
	AnimManager();
	virtual ~AnimManager();

	// アニメーションの更新
	void Update();

	// モデルハンドルの取得
	int GetModelHandle() const { return _modelHandle; }
	// モデルハンドルの設定
	//	既存のアタッチ済みアニメーションはデタッチして切り替える
	void SetModelHandle(int handle);

	// アニメーションが終了したか
	bool IsAnimationFinished() const;

	// 現在のアニメーション番号
	int GetCurrentAnimIndex() const { return _currentAnimIndex; }

	// アニメーション名からインデックスを取得してアニメーションを切り替え
	bool ChangeAnimationByName(const char* animName, float blendFrame, int loopCnt, float playSpeed = 1.0f);

	// 管理中のアニメーションのクリア
	void Release();

	// 現在のアニメーションの総再生時間を取得
	float GetCurrentAnimTotalTime() const;

private:
	// アニメーション情報
	struct AnimationData
	{
		int attachIndex = -1;// アタッチインデックス
		float totalTime = 0.0f;// 総再生時間
		float playTime = 0.0f;// 現在の再生時間
		float closeTime = 0.0f;// ブレンド終了までの時間
		float closeTotalTime = 0.0f;// ブレンド終了の総時間
		float fadeInTime = 0.0f;// フェードイン時間
		float fadeInTotalTime = 0.0f;// フェードインの総時間
		int loopCnt = 0;// ループ回数(0:無限ループ　1:ループ無し　2以上:指定回数ループ)
		float playSpeed = 1.0f;// 再生速度
		bool isFinished = false;// アニメーションが終わったか
	};

	int _modelHandle = -1;

	std::vector<std::unique_ptr<AnimationData>> _activeAnims;
	int _currentAnimIndex = -1;// 現在のアニメーションインデックス

	// アクティブなアニメーションのクリア
	void ClearActiveAnimations();
	// アニメーションのデタッチ
	void DetachAnimation(AnimationData* anim);
};

