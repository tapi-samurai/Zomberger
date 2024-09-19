#pragma once
#include "Node.h"
#include "Vector3.h"
#include "Quaternion.h"

// ƒJƒƒ‰‚ÌŠî’êƒNƒ‰ƒX
class Camera : public Node
{
protected:
	Vector3 m_position;
	Quaternion m_rotation;

	float m_setNear;	// Å’Z•`‰æ‹——£
	float m_setFar;		// Å’·•`‰æ‹——£
	float m_fov;		// ‹–ìŠp

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