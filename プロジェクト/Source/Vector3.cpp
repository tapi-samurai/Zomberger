#include "Vector3.h"
#include "Vector2.h"
#include <cmath>

// Vector2へのキャスト
Vector3::operator Vector2() const
{
	return Vector2(x, y);
}

// VECTORへのキャスト
Vector3::operator VECTOR() const
{
	return VGet(x, y, z);
}

// 長さを取得
float Vector3::Magnitude() const
{
	return std::sqrtf(SqrMagnitude());
}

// ２点間の距離を取得
float Vector3::Distance(const Vector3& from, const Vector3& to)
{
	return std::sqrtf((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y) + (from.z - to.z) * (from.z - to.z));
}

// 正規化したベクトルを取得
Vector3 Vector3::Normalize() const 
{
	if (Vector3(x, y, z) == Vector3())
	{
		return Vector3();
	}

	float length = Magnitude();

	return Vector3(x, y, z) / length;
}

// 外積を取得
Vector3 Vector3::Cross(const Vector3& from, const Vector3& to)
{
	Vector3 result;
	result.x = from.y * to.z - from.z * to.y;
	result.y = from.z * to.x - from.x * to.z;
	result.z = from.x * to.y - from.y * to.x;

	return result;
}

// 内積を取得
float Vector3::Dot(const Vector3& from, const Vector3& to)
{
	return from.x * to.x + from.y * to.y + from.z * to.z;
}

// ２つのベクトルの角度
float Vector3::Angle(const Vector3& from, const Vector3& to)
{
	return Vector3::Dot(from, to) / (sqrtf(from.x * from.x + from.y * from.y + from.z * from.z) * sqrtf(to.x * to.x + to.y * to.y + to.z * to.z));
}

// 線形補間
Vector3 Vector3::Lerp(const Vector3& begin, const Vector3& end, float t)
{
	return begin * (1.0f - t) + end * t;
}