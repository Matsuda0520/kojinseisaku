#include "CharacterBase.h"

CharacterBase::CharacterBase(const char* name) 
	: GameComposite(name)
	, _hp(100.0f)
	, _maxHp(100.0f)
	, _speed(1.0f)
	, _modelHandle(-1)
{
}

CharacterBase::~CharacterBase()
{
	// 自身が破棄される前に、オブザーバーに通知しておく
	for (auto* observer : _observers)
	{
		if (observer)
		{
			observer->OnTargetDestroyed();
		}
	}
	_observers.clear();
}

void CharacterBase::Initialize()
{
}

void CharacterBase::Process()
{
	if (_isDead) { return; }

	Move();

	// 子のProcessを呼び出す
	GameComposite::Process();
}

void CharacterBase::Render()
{
	if (_isDead) { return; }

	if (_modelHandle != -1)
	{
		// モデルの描画
		// 拡大行列を作成
		MATRIX mScale = MGetScale(ToDX(_scale));

		// 回転行列を作成
		MATRIX mRotX = MGetRotX(_rot.GetX());
		MATRIX mRotY = MGetRotY(_rot.GetY());
		MATRIX mRotZ = MGetRotZ(_rot.GetZ());

		// 平行移動行列を作成
		MATRIX mTrans = MGetTranslate(ToDX(_pos));

		// 拡大 -> 回転 -> 平行移動の順に行列を掛け合わせる
		MATRIX m = MGetIdent();
		m = MMult(m, mScale);
		m = MMult(m, mRotX);// X, Y, Zの順で回転する
		m = MMult(m, mRotY);
		m = MMult(m, mRotZ);
		m = MMult(m, mTrans);// 最後に移動する

		// モデルに行列をセット
		MV1SetMatrix(_modelHandle, m);

		// モデルの描画
		MV1DrawModel(_modelHandle);
	}

	// test
	DrawSphere3D(ToDX(_pos), 10.0f, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

	// 子のRenderを呼び出す
	GameComposite::Render();
}

void CharacterBase::Terminate()
{
	if (_modelHandle != -1)
	{
		// モデルの解放
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

void CharacterBase::TakeDamage(float damage)
{
	if (_isDead) { return; }

	float prevHp = _hp;// ダメージ前のHpを保存
	_hp -= damage;

	// Hpが減少した場合に通知する
	if (_hp < prevHp)
	{
		NotifyHPChanged();
	}

	if (_hp <= 0.0f)
	{
		_hp = 0.0f;

		// このダメージで死亡した場合に通知する
		if (prevHp > 0.0f)
		{
			NotifyDeath();
		}

		Destroy();// 破棄フラグを立てる
	}
}

// オブザーバーの追加
void CharacterBase::AddObserver(IHPObserver* observer)
{
	if (!observer) { return; }

	auto it = std::find(_observers.begin(), _observers.end(), observer);
	if (it == _observers.end())
	{
		_observers.push_back(observer);
	}
}

// オブザーバーの削除
void CharacterBase::RemoveObserver(IHPObserver* observer)
{
	if (!observer) { return; }

	_observers.erase(
		std::remove(
			_observers.begin(), _observers.end(), observer),
		_observers.end()
	);
}

// Hp変更の通知
void CharacterBase::NotifyHPChanged()
{
	for (auto observer : _observers)
	{
		if (observer)
		{
			// 現在のHpと最大Hpを渡す
			observer->OnHPChanged(_hp, _maxHp);
		}
	}
}

// 死亡の通知
void CharacterBase::NotifyDeath()
{
	for (auto observer : _observers)
	{
		if (observer)
		{
			observer->OnDied();
		}
	}
}
