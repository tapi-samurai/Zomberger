﻿#pragma once

class Vector3;

class Vector2
{
public:
	float x;
	float y;
	
	// コンストラクタ
	constexpr Vector2() :
		x(0),
		y(0) {}

	template <typename T1, typename T2>
	constexpr Vector2(T1 x, T2 y) :
		x(static_cast<float>(x)),
		y(static_cast<float>(y)) {}

	// Vector3へのキャスト
	operator Vector3() const;

	// 加算
	constexpr Vector2 operator + (const Vector2& other) const	// Vector2 + Vector2
	{
		return Vector2(x + other.x, y + other.y);
	}
	constexpr Vector2& operator += (const Vector2& other)	// Vector2 += Vector2
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	// 減算
	constexpr Vector2 operator - (const Vector2& other) const	// Vector2 + Vector2
	{
		return Vector2(x - other.x, y - other.y);
	}
	constexpr Vector2& operator -= (const Vector2& other)	// Vector2 -= Vector2
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// 乗算
	constexpr Vector2 operator * (const float& scalar) const	// Vector2 * scalar
	{
		return Vector2(x * scalar, y * scalar);
	}
	constexpr friend Vector2 operator * (const float& scalar, const Vector2& v)	// scalar * Vector2
	{
		return v * scalar;
	}
	constexpr Vector2& operator *= (const float& scalar)	// Vector2 *= scalar
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	// 除算
	constexpr Vector2 operator / (const float& scalar) const	// Vector2 / scalar
	{
		return Vector2(x / scalar, y / scalar);
	}
	constexpr Vector2& operator /= (const float& scalar)	// Vector2 /= scalar
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// 負
	constexpr Vector2 operator - () const	// -Vector2
	{
		return Vector2(-x, -y);
	}

	// 等価
	constexpr bool operator == (const Vector2& other) const	// Vector2 == Vector2
	{
		return (x == other.x) && (y == other.y);
	}

	// 非等価
	constexpr bool operator != (const Vector2& other) const	// Vector2 != Vector2
	{
		return !(*this == other);
	}

	// ゼロ判定
	constexpr bool IsZero() const
	{
		return x == 0 && y == 0;
	}
	
	// 長さの二乗を取得
	constexpr float SqrMagnitude() const
	{
		return x * x + y * y;
	}

	// 長さを取得
	float Magnitude() const;

	// 外積を取得
	static float Cross(const Vector2& from, const Vector2& to);

};
