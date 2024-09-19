#pragma once
#include "Vector2.h"
#include "Quaternion.h"

class Transform
{
public:
	Vector2 position;		// 座標
	Quaternion rotation;	// 角度（直接数値を入れるのはNG）
	float scale;			// 拡縮

	Transform() :
		rotation(Quaternion::CreateIdentity()),
		scale(1) {}
};
