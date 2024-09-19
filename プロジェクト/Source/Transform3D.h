#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class Transform3D
{
public:
	Vector3 position;		// ���W
	Quaternion rotation;	// �p�x�i���ڐ��l������̂�NG�j
	Vector3 scale;			// �g�k

	Transform3D() :
		rotation(Quaternion::CreateIdentity()),
		scale(Vector3(1,1,1)) {}

	const inline Vector3 Forward() const { return Vector3::Forward() * rotation; }
	const inline Vector3 Right() const { return Vector3::Right() * rotation; }
	const inline Vector3 Up() const { return Vector3::Up() * rotation; }
};
