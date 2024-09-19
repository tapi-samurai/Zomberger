#pragma once
#include <cmath>

namespace Math
{
	// �~����
	static constexpr double Dpi = 3.141592653589793;	// 180�x
	static constexpr float Fpi = 3.141592653589793f;

	// �x���烉�W�A���ɕϊ�
	static constexpr double DegToRad(double deg)
	{
		return deg * Dpi / 180.0;
	}
	static constexpr float DegToRad(float deg)
	{
		return static_cast<float>(deg * Dpi / 180.0);
	}

	// ���W�A������x�ɕϊ�
	static constexpr double RadToDeg(double rad)
	{
		return rad * 180.0 / Dpi;
	}

	// ���`���
	template <typename T>
	static constexpr T Lerp(T begin, T end, float t)
	{
		return static_cast<T>(begin * (1.0f - t) + end * t);
	}

	// �l�����͈̔͂ɐ���
	template <typename T>
	static constexpr T Clamp(T value, T min, T max)
	{
		T result;
		result = value > min ? value : min;
		result = value < max ? result : max;
		return result;
	}
};