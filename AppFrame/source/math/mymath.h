#pragma once
#include <algorithm>
#include <random>

#include "Vector2.h"
#include "Vector4.h"

#undef min
#undef max

namespace Math
{
	// degree -> radian
    constexpr float PI = 3.14159265358979323846f;
    constexpr float DEGREE_TO_RADIAN = PI / 180.0f;
    constexpr float RADIAN_TO_DEGREE = 180.0f / PI;

    // 汎用数学　計算ユーティリティ
	// 値をminとmaxの範囲に収める
    template<typename T>
    inline T Clamp(const T& value, const T& min, const T& max)
    {
        return std::max(min, std::min(max, value));
    }

	// 値を0.0fから1.0fの範囲に収める
    inline float Clamp01(float value)
    {
		return Clamp(value, 0.0f, 1.0f);
    }

	// 線形補間 : startからendへのtの割合で補間
    inline float Lerp(float start, float end, float t)
    {
		t = Clamp01(t);
		return start + (end - start) * t;
    }

	// 2Dベクトルの線形補間
    inline Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
    {
		t = Clamp01(t);
		return start + (end - start) * t;
    }

	// 3Dベクトルの線形補間
    inline Vector4 Lerp(const Vector4& start, const Vector4& end, float t)
    {
		t = Clamp01(t);
		return start + (end - start) * t;
    }

	// 角度(ラジアン)を-PIからPIの範囲に正規化
    inline float WrapAngle(float angle)
    {
        while (angle > PI) { angle -= 2.0f * PI; }
		while (angle < -PI) { angle += 2.0f * PI; }
		return angle;
    }

	// minからmaxの範囲でランダムな数値を生成する
    inline float RandomRange(float min, float max)
    {
        thread_local std::mt19937 engine(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(engine);
    }

	// 3DベクトルからY成分を除去し、水平方向の単位ベクトルを返す
    inline Vector4 FlattenVector(const Vector4& v)
    {
        Vector4 flat(v.GetX(), 0.0f, v.GetZ(), 0.0f);

        if (flat.Length() > 0.0001f)
        {
			return flat.Normalize();
        }
		// 長さが極小の場合はゼロベクトルを返す
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}
