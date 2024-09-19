#pragma once
#include "Actor3D.h"

class NoActionObject : public Actor3D
{
public:
	NoActionObject(
		const Vector3& position, const Quaternion& rotation, const Vector3& scale,
		const char* name, const char* modelName, float colliderRadius = 3.0f
	);
};