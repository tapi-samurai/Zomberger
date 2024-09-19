#pragma once
#include "Actor3D.h"

// 3Dモデルを表示するだけのActor3Dクラス

class ModelDisplay : public Actor3D
{
public:
	ModelDisplay(
		const char* name, const char* modelName,
		const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion::CreateIdentity(), const Vector3& scale = Vector3(1, 1, 1)) :
		Actor3D(name, modelName)
	{
		m_transform.position = position;
		m_transform.rotation = rotation;
		m_transform.scale = scale;
	}

	// 座標設定
	void SetPosition(const Vector3& position) { m_transform.position = position; }

	// 回転設定
	void SetRotation(const Quaternion& rotation) { m_transform.rotation = rotation; }

	// 拡縮設定
	void SetScale(const Vector3& scale) { m_transform.scale = scale; }
};