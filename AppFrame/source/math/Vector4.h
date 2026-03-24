#pragma once

namespace Math
{
	class Vector4
	{
	public:
		Vector4();// デフォルトコンストラクタ。(0.0, 0.0, 0.0, 1.0)で初期化
		Vector4(float x, float y, float z);// XYZ成分でベクトルを生成。W成分は1.0
		Vector4(float x, float y, float z, float w);// XYZW成分でベクトルを生成

		Vector4(const Vector4&) = default;// コピーコンストラクタ
		Vector4(Vector4&&) = default;// ムーブコンストラクタ
		virtual ~Vector4() = default;// デストラクタ

		Vector4& operator=(const Vector4&) = default;// コピー代入演算子
		Vector4& operator=(Vector4&&) = default;// ムーブ代入演算子

		// 演算子オーバーロード
		Vector4 operator+(const Vector4& rhs) const;// 2つのベクトルの加算
		Vector4 operator-(const Vector4& rhs) const;// このベクトルからrhsを減算
		Vector4 operator*(float rhs) const;// ベクトルをスカラー倍
		Vector4 operator/(float rhs) const;// ベクトルをスカラー除算

		// ゲッター　セッター
		float GetX() const { return _x; }
		float GetY() const { return _y; }
		float GetZ() const { return _z; }
		float GetW() const { return _w; }

		void SetX(float x) { _x = x; }
		void SetY(float y) { _y = y; }
		void SetZ(float z) { _z = z; }
		void SetW(float w) { _w = w; }
		void Set(const Vector4& vector);// 別のベクトル成分を自身にセット
		void Set(float x, float y, float z);// XYZ成分を直接指定してセット
		void Add(const Vector4& vector);// 別のベクトルを自身に加算
		void Add(float x, float y, float z);// XYZ成分を直接指定して自身に加算

		// ベクトル演算
		float Length() const;// ベクトルの長さを計算
		void Normalized();// ベクトルを正規化
		Vector4 Normalize() const;// 正規化されたベクトルを返す
		float Dot(const Vector4& rhs) const;// 2つのベクトルの内積を計算
		Vector4 Cross(const Vector4& rhs) const;// 2つのベクトルの外積を計算

	private:
		float _x;
		float _y;
		float _z;
		float _w;
	};
}