#pragma once
#include <cmath>

namespace Math
{
	// 円周率
	static constexpr double Dpi = 3.141592653589793;	// 180度
	static constexpr float Fpi = 3.141592653589793f;

	// 度からラジアンに変換
	static constexpr double DegToRad(double deg)
	{
		return deg * Dpi / 180.0;
	}
	static constexpr float DegToRad(float deg)
	{
		return static_cast<float>(deg * Dpi / 180.0);
	}

	// ラジアンから度に変換
	static constexpr double RadToDeg(double rad)
	{
		return rad * 180.0 / Dpi;
	}

	// 線形補間
	template <typename T>
	static constexpr T Lerp(T begin, T end, float t)
	{
		return static_cast<T>(begin * (1.0f - t) + end * t);
	}

	// 値を一定の範囲に制限
	template <typename T>
	static constexpr T Clamp(T value, T min, T max)
	{
		T result;
		result = value > min ? value : min;
		result = value < max ? result : max;
		return result;
	}
};