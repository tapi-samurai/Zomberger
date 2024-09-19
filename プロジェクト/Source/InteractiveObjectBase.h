#pragma once
#include "Actor3D.h"

class InteractiveObjectBase : public Actor3D
{
protected:
	Vector3 m_playerPos;
	bool m_isNear;

	void Update() override;

	void OnCollision3D(const Actor3D* other) override;

	virtual void OnInteract() = 0;	// インタラクト時の処理

public:
	InteractiveObjectBase(Collider3D* collider, const char* name = "", const char* modelName = nullptr);
};