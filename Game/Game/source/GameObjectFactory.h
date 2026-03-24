#pragma once
#include "appframe.h"
#include "GameObject.h"

class GameObjectFactory
{
public:
	// オブジェクトを生成する関数の型
	using CreatorFunc = std::function<std::unique_ptr<GameObject>()>;

	// シングルトンインスタンスの取得
	static GameObjectFactory& GetInstance()
	{
		static GameObjectFactory instance;
		return instance;
	}

	GameObjectFactory(const GameObjectFactory&) = delete;
	GameObjectFactory& operator=(const GameObjectFactory&) = delete;

	void Register(const std::string& typeName, CreatorFunc creator)
	{
		_creators[typeName] = creator;
	}

	// マップにクラス名と生成関数を登録する
	template<typename T>
	void RegisterType(const std::string& typeName)
	{
		Register(typeName
			, [typeName]()
			{
				// 指定された型のインスタンスを生成して返す
				return std::make_unique<T>(typeName.c_str());
			}
		);
	}

	// クラス名を受け取り、対応するオブジェクトを生成して返す
	std::unique_ptr<GameObject> Create(const std::string& typeName)
	{
		// マップを探索し、与えられたクラス名を探す
		auto it = _creators.find(typeName);
		if (it != _creators.end())
		{
			// 登録された生成関数を呼び出して返す
			return it->second();
		}

		// 登録されていないクラス名の場合はnullptrを返す
		return nullptr;
	}

private:
	GameObjectFactory() = default;
	~GameObjectFactory() = default;

	// クラス名と、そのクラスを生成する関数のマップ
	std::unordered_map<std::string, CreatorFunc> _creators;
};

