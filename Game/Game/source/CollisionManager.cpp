#include "CollisionManager.h"

CollisionManager::CollisionManager(const char* name)
	: GameObject(name)
{
}

void CollisionManager::Process()
{
	if (_isDead) { return; }

	size_t count = _colliders.size();
	if (count < 2) { return; }// 判定対象が2つ未満なら処理しない

	// このフレームの衝突ペア
	std::set<std::pair<ICollider*, ICollider*>> currentHits;

	// 全ペアの当たり判定をチェックする
	for (size_t i = 0; i < count - 1; ++i)
	{
		ICollider* colA = _colliders[i];
		GameObject* ownerA = colA->GetOwner();

		// オーナーが死亡フラグを立てていたら判定をスキップ
		if (!ownerA || ownerA->IsDead()) { continue; }

		for(size_t j = i + 1; j < count; ++j)
		{
			ICollider* colB = _colliders[j];
			GameObject* ownerB = colB->GetOwner();

			// オーナーがいない、死亡している、同じオーナー同士ならスキップ
			if (!ownerB || ownerB->IsDead() || ownerA == ownerB) { continue; }

			// 衝突するレイヤーの組み合わせかチェック
			if (!CanCollide(colA->GetLayer(), colB->GetLayer())) { continue; }

			// 衝突判定
			if (CheckCollision(colA, colB))
			{
				// アドレスが小さいほうをfirst、大きいほうをsecondにして
				ICollider* first = std::min(colA, colB);
				ICollider* second = std::max(colA, colB);
				// このペアをセットに追加する
				currentHits.insert({ first, second });
			}
		}
	}

	// 状態の比較とイベントの呼び出し
	for (const auto& hitPair : currentHits)
	{
		if (_prevHits.find(hitPair) == _prevHits.end())
		{
			// 前フレームは無くて、今フレームはある場合は当たった瞬間
			hitPair.first->OnCollisionEnter(hitPair.second->GetOwner());
			hitPair.second->OnCollisionEnter(hitPair.first->GetOwner());
		}
		else
		{
			// 前フレームも、今フレームもある場合は当たっている間
			hitPair.first->OnCollisionStay(hitPair.second->GetOwner());
			hitPair.second->OnCollisionStay(hitPair.first->GetOwner());
		}
	}

	for (const auto& hitPair : _prevHits)
	{
		if (currentHits.find(hitPair) == currentHits.end())
		{
			// 前フレームはあって、今フレームはない場合は離れた瞬間
			hitPair.first->OnCollisionExit(hitPair.second->GetOwner());
			hitPair.second->OnCollisionExit(hitPair.first->GetOwner());
		}
	}

	// 履歴を更新
	_prevHits = currentHits;
}

void CollisionManager::Render()
{
	// デバッグ用にコライダーを描画
	unsigned int color = GetColor(0, 255, 0);

	for (ICollider* col : _colliders)
	{
		// 死亡しているオーナーのコライダーは描画しない
		GameObject* owner = col->GetOwner();
		if (!owner || owner->IsDead()) { continue; }

		CollisionShape shape = col->GetShapeType();

		if (shape == CollisionShape::Capsule)
		{
			// カプセルの描画
			auto cap = col->AsCapsuleCollider();
			if (cap)
			{
				DrawCapsule3D(
					ToDX(cap->GetCapsuleStart()),
					ToDX(cap->GetCapsuleEnd()),
					cap->GetCapsuleRadius(),
					12, color, color, FALSE
				);
			}
		}
	}
}

void CollisionManager::Terminate()
{
	_colliders.clear();
}

void CollisionManager::Register(ICollider* collider)
{
	if (!collider) { return; }

	auto it = std::find(_colliders.begin(), _colliders.end(), collider);
	if(it == _colliders.end())
	{
		_colliders.push_back(collider);
	}
}

void CollisionManager::Unregister(ICollider* collider)
{
	if (!collider) { return; }

	_colliders.erase(
		std::remove(_colliders.begin(), _colliders.end(), collider),
		_colliders.end()
	);

	// 履歴から解除するコライダーを含むペアを削除する
	for (auto it = _prevHits.begin(); it != _prevHits.end();)
	{
		if (it->first == collider || it->second == collider)
		{
			it = _prevHits.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool CollisionManager::CheckCollision(ICollider* a, ICollider* b)
{
	// 当たり判定
	bool isHit = false;

	CollisionShape shapeA = a->GetShapeType();
	CollisionShape shapeB = b->GetShapeType();

	// カプセル同士の判定
	if (shapeA == CollisionShape::Capsule && shapeB == CollisionShape::Capsule)
	{
		auto capA = a->AsCapsuleCollider();
		auto capB = b->AsCapsuleCollider();

		if(capA && capB)
		{
			// まず、大まかな判定で当たっていなければ処理しない
			if (!CheckCapsuleCapsuleRough(capA, capB)) { return false; }
			// カプセル同士の詳細な判定
			isHit = CheckCapsuleCapsule(capA, capB);
		}
	}

	// 球同士の判定

	return isHit;
}

bool CollisionManager::CanCollide(CollisionLayer a, CollisionLayer b) const
{
	// 設定したレイヤーペアに当てはまるかチェック
	for (const auto& pair : s_collisonPairs)
	{
		if ((pair.first == a && pair.second == b) ||
			(pair.first == b && pair.second == a))
		{
			return true;
		}
	}

	return false;
}

bool CollisionManager::CheckSphereSphere(const ISphereCollider* a, const ISphereCollider* b) const
{
	// 中心間距離と半径の和を比較して当たり判定
	return HitCheck_Sphere_Sphere(
		ToDX(a->GetSphereCenter()),
		a->GetSphereRadius(),
		ToDX(b->GetSphereCenter()),
		b->GetSphereRadius()) != 0;
}

bool CollisionManager::CheckCapsuleCapsule(const ICapsuleCollider* a, const ICapsuleCollider* b) const
{
	// 2本の線分と半径を比較して当たり判定
	return HitCheck_Capsule_Capsule(
		ToDX(a->GetCapsuleStart()),
		ToDX(a->GetCapsuleEnd()),
		a->GetCapsuleRadius(),
		ToDX(b->GetCapsuleStart()),
		ToDX(b->GetCapsuleEnd()),
		b->GetCapsuleRadius()) != 0;
}

bool CollisionManager::CheckCapsuleCapsuleRough(const ICapsuleCollider* a, const ICapsuleCollider* b) const
{
	// カプセルを内包する球で大まかに当たり判定
	VECTOR centerA, centerB;
	float radiusA, radiusB;

	// カプセルを内包する球を計算
	CalcCapsuleRoughSphere(a, centerA, radiusA);
	CalcCapsuleRoughSphere(b, centerB, radiusB);

	return HitCheck_Sphere_Sphere(
		centerA, radiusA,
		centerB, radiusB) != 0;
}

void CollisionManager::CalcCapsuleRoughSphere(const ICapsuleCollider* capsule, VECTOR& outCenter, float& outRadius) const
{
	// カプセルの中心と半径を計算する
	Vector4 start = capsule->GetCapsuleStart();
	Vector4 end = capsule->GetCapsuleEnd();
	Vector4 axis = end - start;
	Vector4 center = (start + end) * 0.5f;

	// 球の中心はカプセルの中心、半径はカプセル半径 + 軸の長さの半分
	outCenter = ToDX(center);
	outRadius = capsule->GetCapsuleRadius() + (axis.Length() * 0.5f);
}
