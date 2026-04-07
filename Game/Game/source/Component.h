#pragma once

class GameObject;

// コンポーネントの基底クラス
// 具体的な機能を部品として定義するたためのインターフェース
class Component
{
public:
	virtual ~Component() = default;

	// ゲームオブジェクトの更新に合わせて呼び出される
	virtual void Initialize() {}
	virtual void Process() {}
	virtual void Render() {}
	virtual void Terminate() {}

	// オーナーの取得。自身がどのオブジェクトにアタッチされているか
	GameObject* GetOwner() const { return _owner; }

private:
	// friend:GameObjectクラスがprivateメンバにアクセスできるようにする
	friend class GameObject;
	GameObject* _owner = nullptr;// オーナーへの参照

};

