#pragma once
#include <vector>
#include <memory>
#include <string>

#include "ModeBase.h"

class ModeServer
{
public:
	static ModeServer& GetInstance()
	{
		static ModeServer instance;
		return instance;
	}

	ModeServer(const ModeServer&) = delete;
	ModeServer& operator=(const ModeServer&) = delete;

	int Add(std::unique_ptr<ModeBase> mode, int layer, const char* name);// 登録予約
	void Del(ModeBase* mode);// 削除予約

	ModeBase* Get(int uid);
	ModeBase* Get(const char* name);

	void Clear();// 全削除

	static constexpr int LayerTop() { return INT32_MAX; }

	int ProcessInit();// プロセスを回すための初期化
	int Process();// レイヤーの上の方から処理
	int ProcessFinish();// プロセスを回した後の後始末

	int RenderInit();// 描画を回すための初期化
	int Render();// レイヤーの下の方から処理
	int RenderFinish();// 描画を回した後の後始末

	int SkipProcessUnderLayer();// 現Processで、今処理しているレイヤーより下のレイヤーは、処理を呼ばない
	int SkipRenderUnderLayer();// 現Processで、今処理しているレイヤーより下のレイヤーは、描画を呼ばない
	int PauseProcessUnderLayer();// 現Processで、今処理しているレイヤーより下のレイヤーは、時間経過を止める

private:
	ModeServer() = default;
	~ModeServer() { Clear(); }

	std::vector<std::unique_ptr<ModeBase>> _modes;// モードリスト
	std::vector<std::unique_ptr<ModeBase>> _modeAdds;// 追加予約

	int _uidCCount = 1;// uidカウンタ

	// 参照用ポインタ
	ModeBase* _nowMode = nullptr;// 現在呼び出し中のモード
	ModeBase* _skipProcessMode = nullptr;// このモードより下はProcessを呼ばない
	ModeBase* _skipRenderMode = nullptr;// このモードより下はRenderを呼ばない
	ModeBase* _pauseProcessMode = nullptr;// このモードより下はProcess時に時間経過をさせない
};