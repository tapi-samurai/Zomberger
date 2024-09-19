#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class Transform3D
{
public:
	Vector3 position;		// 座標
	Quaternion rotation;	// 角度（直接数値を入れるのはNG）
	Vector3 scale;			// 拡縮

	Transform3D() :
		rotation(Quaternion::CreateIdentity()),
		scale(Vector3(1,1,1)) {}

	const inline Vector3 Forward() const { return Vector3::Forward() * rotation; }
	const inline Vector3 Right() const { return Vector3::Right() * rotation; }
	const inline Vector3 Up() const { return Vector3::Up() * rotation; }
};
