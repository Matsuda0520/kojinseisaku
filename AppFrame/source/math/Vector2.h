#pragma once

namespace Math
{
	class Vector2
	{
	public:
		Vector2();// デフォルトコンストラクタ。(0.0, 0.0)で初期化
		Vector2(float x, float y);// XY成分でベクトルを生成

		Vector2(const Vector2&) = default;// コピーコンストラクタ
		Vector2(Vector2&&) = default;// ムーブコンストラクタ
		virtual ~Vector2() = default;// デストラクタ

		Vector2& operator=(const Vector2&) = default;// コピー代入演算子
		Vector2& operator=(Vector2&&) = default;// ムーブ代入演算子

		// 演算子オーバーロード
		Vector2 operator+(const Vector2& rhs) const;
		Vector2 operator-(const Vector2& rhs) const;
		Vector2 operator*(float rhs) const;
		Vector2 operator/(float rhs) const;

		// ゲッター　セッター
		float GetX() const { return _x; }
		float GetY() const { return _y; }

		void Set(const Vector2& vector);
		void Set(float x, float y);

		void Add(const Vector2& vector);
		void Add(float x, float y);

		// ベクトル演算
		float Length() const;// ベクトルの長さを計算
		void Normalized();// ベクトルを正規化
		Vector2 Normalize() const;// 正規化されたベクトルを返す
		float Dot(const Vector2& rhs) const;// 2つのベクトルの内積を計算
		float Cross(const Vector2& rhs) const;// 2D疑似外積を計算。スカラー値を返す

	private:
		float _x;
		float _y;
	};
}