#include "ModeServer.h"
#include <algorithm>
#include "DxLib.h"

int ModeServer::Add(std::unique_ptr<ModeBase> mode, int layer, const char* name)
{
	mode->_uid = _uidCCount++;
	mode->_layer = layer;
	mode->_szName = name ? name : "";

	int assignedUid = mode->_uid;
	_modeAdds.push_back(std::move(mode));
	return assignedUid;
}

void ModeServer::Del(ModeBase* mode)
{
	if (mode) { mode->Destroy(); }
}

void ModeServer::Clear()
{
	for (auto& mode : _modes) { mode->Terminate(); }
	for (auto& mode : _modeAdds) { mode->Terminate(); }
	_modes.clear();
	_modeAdds.clear();
}

// 登録idから検索
ModeBase* ModeServer::Get(int uid)
{
	for (const auto& mode : _modes)
	{
		if(!mode->IsDead()  && mode->GetUid() == uid)
		{
			return mode.get();
		}
	}

	for (const auto& mode : _modeAdds)
	{
		if(!mode->IsDead()  && mode->GetUid() == uid)
		{
			return mode.get();
		}
	}

	return nullptr;
}

// 名前から検索
ModeBase* ModeServer::Get(const char* name)
{
	if (!name) { return nullptr; }

	std::string serchName(name);

	for(const auto& mode : _modes)
	{
		if(!mode->IsDead()  && mode->GetName() == serchName)
		{
			return mode.get();
		}
	}

	for(const auto& mode : _modeAdds)
	{
		if(!mode->IsDead()  && mode->GetName() == serchName)
		{
			return mode.get();
		}
	}

	return nullptr;
}

// プロセスを回すための初期化
int ModeServer::ProcessInit()
{
	// 削除フラフが立っているモードをリストから削除
	_modes.erase(
		std::remove_if(_modes.begin(), _modes.end(),
			[](const std::unique_ptr<ModeBase>& mode)
			{
				if (mode->IsDead())
				{
					mode->Terminate();
					return true;// trueを返すと削除される
				}
				return false;
			}), 
		_modes.end());

	// Addされたモードの有効化
	if (!_modeAdds.empty())
	{
		for (auto& mode : _modeAdds)
		{
			// 追加待ちの間に削除予約されていなければリストに追加
			if (!mode->IsDead())
			{
				mode->Initialize();
				_modes.push_back(std::move(mode));
			}
		}
	}
	_modeAdds.clear();

	// レイヤー順にソート
	std::sort(_modes.begin(), _modes.end(),
		[](const std::unique_ptr<ModeBase>& a, const std::unique_ptr<ModeBase>& b)
		{
			return a->_layer < b->_layer;
		});

	// スキップ、ポーズの解除
	_skipProcessMode = nullptr;
	_skipRenderMode = nullptr;
	_pauseProcessMode = nullptr;

	return 0;
}

int ModeServer::Process()
{
	// 現在の時間を取得
	unsigned int t = GetNowCount();
	bool pause = false;

	// レイヤーの上の方から処理
	for (auto it = _modes.begin(); it != _modes.end(); ++it)
	{
		auto& mode = *it;
		if (mode->IsDead()) { continue; }

		_nowMode = mode.get();

		if (!pause)
		{
			// 時間経過処理
			mode->StepTime(t);
		}

		// Processコール
		mode->Process();

		// modeがない場合
		if (_modes.empty()) { break; }

		if (!pause)
		{
			// カウンタ処理
			mode->StepCount();
		}

		if (_skipProcessMode == mode.get()) { break; }
		if (_pauseProcessMode == mode.get()) { pause = true; }
	}

	_nowMode = nullptr;

	return 0;
}

int ModeServer::ProcessFinish() { return 0; }

int ModeServer::RenderInit() { return 0; }

int ModeServer::Render()
{
	// レイヤーの下の方から処理
	for (const auto& mode : _modes)
	{
		if (_skipRenderMode && _skipRenderMode != mode.get()) { continue; }
		_skipRenderMode = nullptr;

		if(!mode->IsDead())
		{
			_nowMode = mode.get();
			mode->Render();
		}
	}
	_nowMode = nullptr;
		
	return 0;
}

int ModeServer::RenderFinish() { return 0; }



// 今処理しているレイヤーより下のレイヤーは、処理を呼ばない
int ModeServer::SkipProcessUnderLayer()
{
	_skipProcessMode = _nowMode;
	return 0;
}

// 今処理しているレイヤーより下のレイヤーは、描画を呼ばない
int ModeServer::SkipRenderUnderLayer()
{
	_skipRenderMode = _nowMode;
	return 0;
}


// 今処理しているレイヤーより下のレイヤーは、時間経過を止める
int ModeServer::PauseProcessUnderLayer()
{
	_pauseProcessMode = _nowMode;
	return 0;
}