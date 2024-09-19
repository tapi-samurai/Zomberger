#pragma once
#include "Node.h"
#include "Vector3.h"
#include "Quaternion.h"

// �J�����̊��N���X
class Camera : public Node
{
protected:
	Vector3 m_position;
	Quaternion m_rotation;

	float m_setNear;	// �ŒZ�`�拗��
	float m_setFar;		// �Œ��`�拗��
	float m_fov;		// ����p

	void Draw() override;

public:
	Camera(
		const Vector3& position = Vector3(0, 10, -30), const Quaternion& rotation = Quaternion::CreateIdentity(),
		float setNear = 1.0f, float setFar = 100.0f, float fov = 60.0f
	) :
		m_position(position),
		m_rotation(rotation),
		m_setNear(setNear),
		m_setFar(setFar),
		m_fov(fov) {}

	const Vector3& GetPosition() const { return m_position; }

	const Quaternion& GetRotation() const { return m_rotation; }
};