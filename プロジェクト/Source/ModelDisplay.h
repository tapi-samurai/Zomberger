#pragma once
#include "Actor3D.h"

// 3D���f����\�����邾����Actor3D�N���X

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

	// ���W�ݒ�
	void SetPosition(const Vector3& position) { m_transform.position = position; }

	// ��]�ݒ�
	void SetRotation(const Quaternion& rotation) { m_transform.rotation = rotation; }

	// �g�k�ݒ�
	void SetScale(const Vector3& scale) { m_transform.scale = scale; }
};