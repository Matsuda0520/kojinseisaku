#include "AnimManager.h"

AnimManager::AnimManager()
	: _modelHandle(-1)
	, _currentAnimIndex(-1)
{
}

AnimManager::~AnimManager()
{
	Release();
}

void AnimManager::Update()
{
	if (_modelHandle == -1) { return; }

	// アクティブなアニメーションを更新
	auto it = _activeAnims.begin();
	while (it != _activeAnims.end())
	{
		AnimationData* anim = it->get();

		// 再生時間を進める
		anim->playTime += anim->playSpeed;

		anim->isFinished = false;

		// ループ処理
		if (anim->totalTime > 0.0f && anim->playTime >= anim->totalTime)
		{
			if (anim->loopCnt == 0)
			{
				// 無限ループ
				anim->playTime = std::fmod(anim->playTime, anim->totalTime);
			}
			else if (anim->loopCnt > 1)
			{
				// 指定回数ループ
				--anim->loopCnt;
				anim->playTime = std::fmod(anim->playTime, anim->totalTime);
			}
			else
			{
				// ループ終了
				anim->playTime = anim->totalTime;
				anim->isFinished = true;
			}
		}

		// アニメーション時間を設定
		MV1SetAttachAnimTime(_modelHandle, anim->attachIndex, anim->playTime);

		// ブレンド率を計算
		float blendRate = 1.0f;

		// フェードアウト処理
		if (anim->closeTime > 0.0f)
		{
			anim->closeTime -= 1.0f;

			if (anim->closeTotalTime > 0.0f)
			{
				blendRate = anim->closeTime / anim->closeTotalTime;
			}
			else
			{
				blendRate = 0.0f;
			}
		}
		// フェードイン処理
		else if (anim->fadeInTotalTime > 0.0f && anim->fadeInTime < anim->fadeInTotalTime)
		{
			anim->fadeInTime += 1.0f;
			blendRate = anim->fadeInTime / anim->fadeInTotalTime;
		}

		// ブレンド率の範囲制限
		blendRate = Math::Clamp01(blendRate);

		// ブレンド率を設定
		MV1SetAttachAnimBlendRate(_modelHandle, anim->attachIndex, blendRate);

		// フェードアウト完了したアニメーションをデタッチ
		if (anim->closeTime <= 0.0f && anim->closeTotalTime > 0.0f)
		{
			DetachAnimation(anim);
			it = _activeAnims.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void AnimManager::SetModelHandle(int handle)
{
	if (_modelHandle == handle)
	{
		return;
	}

	ClearActiveAnimations();

	_modelHandle = handle;
	_currentAnimIndex = -1;
}

bool AnimManager::ChangeAnimationByName(const char* animName, float blendFrame, int loopCnt, float playSpeed)
{
	if (_modelHandle == -1) { return false; }
	if (animName == nullptr) { return false; }

	// モデル内のアニメーション名からインデックスを取得
	int animIndex = MV1GetAnimIndex(_modelHandle, animName);
	if (animIndex == -1)
	{
		// アニメーションが見つからなかった
		return false;
	}

	// 既存アニメーションをフェードアウトへ移行
	for (auto& anim : _activeAnims)
	{
		if (anim->closeTime <= 0.0f)
		{
			float currentBlendRate = 1.0f;

			// フェードイン途中なら現在ブレンド率を反映
			if (anim->fadeInTotalTime > 0.0f && anim->fadeInTime < anim->fadeInTotalTime)
			{
				currentBlendRate = anim->fadeInTime / anim->fadeInTotalTime;
			}

			currentBlendRate = Math::Clamp01(currentBlendRate);
			anim->closeTime = blendFrame * currentBlendRate;
			anim->closeTotalTime = blendFrame;
		}
	}

	// 新しいアニメーションを追加
	auto newAnim = std::make_unique<AnimationData>();
	newAnim->attachIndex = MV1AttachAnim(_modelHandle, animIndex, -1, FALSE);
	if (newAnim->attachIndex < 0)
	{
		return false;
	}

	newAnim->totalTime = MV1GetAttachAnimTotalTime(_modelHandle, newAnim->attachIndex);
	newAnim->fadeInTotalTime = (blendFrame > 0.0f) ? blendFrame : 0.0f;
	newAnim->loopCnt = loopCnt;
	newAnim->playSpeed = playSpeed;

	// 即切り替えなら1.0、通常は0.0からフェードイン
	float startBlendRate = (blendFrame <= 0.0f) ? 1.0f : 0.0f;
	MV1SetAttachAnimBlendRate(_modelHandle, newAnim->attachIndex, startBlendRate);

	_activeAnims.push_back(std::move(newAnim));
	_currentAnimIndex = animIndex;

	return true;
}

bool AnimManager::IsAnimationFinished() const
{
	// アクティブなアニメーションがない場合はtrueを返す
	if (_activeAnims.empty()) { return true; }

	// 最新アニメーションの終了状態
	return _activeAnims.back()->isFinished;
}

void AnimManager::Release()
{
	// 管理中のアニメーションのみ解放
	ClearActiveAnimations();
	_currentAnimIndex = -1;
}

// アニメーションの総再生時間を取得
float AnimManager::GetCurrentAnimTotalTime() const
{
	// アクティブなアニメーションがない場合は0を返す
	if(_activeAnims.empty()) { return 0.0f; }

	// 最新のアニメーションの総再生時間を返す
	return _activeAnims.back()->totalTime;
}

void AnimManager::ClearActiveAnimations()
{
	for(auto& anim : _activeAnims)
	{
		DetachAnimation(anim.get());
	}
	_activeAnims.clear();
}

void AnimManager::DetachAnimation(AnimationData* anim)
{
	if (_modelHandle == -1 || anim == nullptr)
	{
		return;
	}

	if (anim->attachIndex < 0)
	{
		return;
	}

	MV1DetachAnim(_modelHandle, anim->attachIndex);
}
