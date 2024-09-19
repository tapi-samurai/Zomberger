#pragma once
#include "Transform3D.h"
#include "Collision3D.h"

class SphereCollider3D;
class CapsuleCollider3D;
class QuadCollider3D;

// ���ۂ̓����蔻��̏������s��
class Collider3D
{
public:
	Vector3 positionOffset;	// ���S���W����̃Y��

	bool isEnabled;		// �R���W�������L����

	// �R���X�g���N�^
	Collider3D(const Vector3& positionOffset) :
		positionOffset(positionOffset),
		isEnabled(true) {}

	// �Փ˔���
	// �_�u���f�B�X�p�b�`�ɂ��`�󎯕�
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const = 0;

#ifdef _DEBUG
	// �����蔻��̕\��
	virtual void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const = 0; 
#endif

	// ���a�̎擾
	virtual const float GetRadius() const { return 0; }

	// �������擾
	virtual const float GetWidth() const { return 0; }
};