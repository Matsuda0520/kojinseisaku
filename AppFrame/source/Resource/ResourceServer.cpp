#include "ResourceServer.h"

void ResourceServer::Register(const std::string& name, const char* path, ResourceType type, float scale)
{
	ResourceInfo info;
	info.name = name;
	info.path = path;
	info.type = type;
	info.scale = scale;
	info.handle = -1;
	_loads.push_back(info);
}

void ResourceServer::StartLoadAsync()
{
	_totalCount = static_cast<int>(_loads.size());// ロード待ちのリソースの総数を保存

	// DXライブラリの非同期ロードをONにする
	SetUseASyncLoadFlag(TRUE);

	// 先頭から順番にロード開始
	for (auto& res : _loads)
	{
		int handle = -1;

		switch (res.type)
		{
			case ResourceType::Graph:
				break;
			case ResourceType::Model:
				break;
			case ResourceType::Effect:
				break;
			case ResourceType::Sound:
				break;
			default:
				break;
		}

		if (handle != -1)
		{
			_handleMap[res.name] = handle;// マップに登録
			res.handle = handle;
		}
	}

	// 非同期ロードをOFFに戻す
	SetUseASyncLoadFlag(FALSE);
}

float ResourceServer::GetLoadProgress() const
{
	if (_totalCount == 0) { return 1.0f; }// ロード待ちがない

	int remain = GetASyncLoadNum();// 非同期ロード中の処理の数を取得
	return static_cast<float>(_totalCount - remain) / static_cast<float>(_totalCount);
}

bool ResourceServer::IsLoadComplete() const
{
	return GetASyncLoadNum() == 0;// 非同期ロード中の処理の数が0なら完了
}

void ResourceServer::Terminate()
{
	// DxLibで確保したメモリを開放する
	for (const auto& res : _loads)
	{
		if (res.handle > 0)// ハンドルが有効な場合のみ
		{
			switch (res.type)
			{
				case ResourceType::Graph:
					DeleteGraph(res.handle);
					break;
				case ResourceType::Model:
					MV1DeleteModel(res.handle);
					break;
				default:
					break;
			}
		}
	}

	_loads.clear();
	_handleMap.clear();
	_totalCount = 0;
}

int ResourceServer::GetHandle(const std::string& name) const
{
	auto it = _handleMap.find(name);
	if (it != _handleMap.end())
	{
		return it->second;
	}

	return -1;// 見つからない場合は-1を返す
}
