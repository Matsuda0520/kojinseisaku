#pragma once
#include "appframe.h"
#include "Component.h"

// ゲームオブジェクトの基底クラス
// ゲームオブジェクトは、コンポーネントを持ち、親子関係を構築する
class GameObject
{
public:
	GameObject(const char* name = "GameObject");
	virtual ~GameObject() = default;

	virtual void Initialize();
	virtual void Terminate();
	virtual void Process();
	virtual void Render();

	// 親オブジェクトの取得。自身がどのオブジェクトの子であるか
	GameObject* GetParent() const { return _parent; }
	// 親オブジェクトの設定。AddChildで使用する
	void SetParent(GameObject* parent) { _parent = parent; }

	const char* GetName() const { return _name.c_str(); }
	bool IsDead() const { return _isDead; }// 死亡フラグの確認

	Vector4 GetPosition() const { return _position; }
	Vector4 GetRotation() const { return _rotation; }
	Vector4 GetScale() const { return _scale; }

	// オブジェクトの破棄要求。即座に削除せず、親がまとめて削除するためのフラグを立てる
	virtual void Destroy() { _isDead = true; }

	// 子オブジェクトの追加。Compositeでオーバーライドする
	virtual void AddChild(std::unique_ptr<GameObject> child) {}

	// コンポーネントを追加して、オーナーの設定と初期化を行う
	// T:追加するコンポーネントの型、Args:コンポーネントのコンストラクタに渡す引数
	template<typename T, typename ... Args>
	T* AddComponent(Args&& ... args)
	{
		// インスタンスの生成、std::forwardで引数をそのままの形で渡す
		auto newComponent = std::make_unique<T>(std::forward < Args.(args)...));

		// 戻り値用の生ポインタを保存しておく
		T* rawPtr = newComponent.get();

		// オーナーの設定と初期化
		newComponent->_owner = this;
		newComponent->Initialize();

		// コンポーネントの所有権をGameObjectに移す
		_components.push_back(std::move(newComponent));

		// 生ポインタを返す
		return rawPtr;
	}

	// 指定したコンポーネントを持っているか確認し、持っていればそのポインタを返す
	template<class T>
	T* GetComponent() const
	{
		for (const autoo& component : _components)
		{
			// dynamic_castを使って、コンポーネントがT型かどうかを確認
			if (T* target = dynamic_cast<T*>(component.get()));
			{
				// T型のコンポーネントが見つかった場合は、そのポインタを返す
				return target;
			}
		}
		// 見つからなかった場合はnullptrを返す
		return nullptr;
	}

protected:
	std::string _name;// オブジェクトの名前
	bool _isDead;// 生存フラグ。trueなら破棄される予定とする
	GameObject* _parent;// 自身の親への参照

	// 自身が持つコンポーネントのコンテナ。unique_ptrで所有権を管理する
	std::vector<std::unique_ptr<Component>> _components;

	Vector4 _position;
	Vector4 _rotation;
	Vector4 _scale;

};

