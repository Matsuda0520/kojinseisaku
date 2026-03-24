#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "DxLib.h"

// リソースの種類
enum class ResourceType
{
	Graph,
	Model,
	Effect,
	Sound,
	_EOT_
};

// リソースごとの情報
struct ResourceInfo
{
	std::string name;// 呼び出し名
	std::string path;// ファイルパス
	ResourceType type;// リソースの種類
	int handle;// ロード後のハンドル
	float scale;// スケール
};

class ResourceServer
{
public:
	static ResourceServer& GetInstance()
	{
		static ResourceServer instance;
		return instance;
	}

	ResourceServer(const ResourceServer&) = delete;
	ResourceServer& operator=(const ResourceServer&) = delete;
	ResourceServer(ResourceServer&&) = delete;
	ResourceServer& operator=(ResourceServer&&) = delete;

	// 登録(ロード待ちに追加)
	void Register(const std::string& name, const char* path, ResourceType type, float scale = 1.0f);

	// 非同期ロード開始
	void StartLoadAsync();

	// ロード進捗率(0.0f～1.0f)
	float GetLoadProgress() const;

	// ロード完了したか
	bool IsLoadComplete() const;

	// 終了、メモリ開放処理
	void Terminate();

	// ハンドル取得
	int GetHandle(const std::string& name) const;

private:
	ResourceServer() = default;
	~ResourceServer() { Terminate(); }

	std::vector<ResourceInfo> _loads;// ロード待ちのリソース
	std::unordered_map<std::string, int> _handleMap;// 検索用マップ(呼び出し名→ハンドル)
	int _totalCount = 0;// ロード待ちのリソースの総数
};

