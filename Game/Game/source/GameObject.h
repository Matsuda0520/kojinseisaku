#pragma once
#include "appframe.h"

class ICollider;
class IAttacker;

// 共通インターフェース
class GameObject
{
public:
	GameObject(const char* name = "GameObject");
	virtual ~GameObject() = default;

	// 毎フレーム呼び出されることで全体を制御する
	virtual void Initialize() = 0;
	virtual void Process() = 0;
	virtual void Render() = 0;
	virtual void Terminate() = 0;

	// オブジェクトを死亡状態にする。以降はProcessやRenderが呼ばれなくなる
	virtual void Destroy();

	void SetParent(GameObject* parent) { _parent = parent; }
	GameObject* GetParent() const { return _parent; }

	// 子オブジェクトを追加する(GameCompositeで実装)
	virtual void AddChild(std::unique_ptr<GameObject> child) {}

	// 衝突イベントを基底クラスで定義しておく
	virtual void OnCollisionEnter(GameObject* other) {}
	virtual void OnCollisionStay(GameObject* other) {}
	virtual void OnCollisionExit(GameObject* other) {}

	// キャスト削減のため、仮想関数で定義しておく
	virtual ICollider* AsCollider() { return nullptr; }
	virtual const ICollider* AsCollider() const { return nullptr; }
	virtual IAttacker* AsAttacker() { return nullptr; }
	virtual const IAttacker* AsAttacker() const { return nullptr; }
	virtual class CharacterBase* AsCharacter() { return nullptr; }// キャラクター型か

	// ゲッター / セッター
	const char* GetName() const { return _name; }
	bool IsDead() const { return _isDead; }
	Vector4 GetPosition() const { return _pos; }
	Vector4 GetRotation() const { return _rot; }
	Vector4 GetScale() const { return _scale; }

	void SetPosition(const Vector4& pos) { _pos = pos; }
	void SetRotation(const Vector4& rot) { _rot = rot; }
	void SetScale(const Vector4& scale) { _scale = scale; }

protected:
	const char* _name;
	bool _isDead;
	GameObject* _parent;// 親への参照

	// 3Dの基本パラメータ
	Vector4 _pos;
	Vector4 _rot;
	Vector4 _scale;
};

