#pragma once
#include "Collider3D.h"
#include "Vector3.h"
#include "DxLib.h"

class CapsuleCollider3D : public Collider3D
{
public:
	Vector3 pos2offset;	// �J�v�Z���̈�_�̍��W�I�t�Z�b�g
	float radius;		// ���̔��a

	// �R���X�g���N�^
	CapsuleCollider3D(const Vector3& positionOffset = Vector3(), Vector3 pos2offset = Vector3(), float radius = 1) :
		Collider3D(positionOffset),
		pos2offset(pos2offset),
		radius(radius) {}

	// �Փ˔���
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const override
	{
		// �_�u���f�B�X�p�b�`�ɂ��`�󔻒�
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const override
	{
		// ���~�J�v�Z��
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const override
	{
		// �J�v�Z���~�J�v�Z��
		return Collision3D::Check(transform1, this, transform2, collider2);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const override
	{
		// �N�A�b�h�~�J�v�Z��
		return Collision3D::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	//�f�o�b�O���ɓ����蔻���`��
	void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const
	{
		// �����蔻�肪�L���ł���ꍇ�̂ݕ\��
		if (isEnabled)
		{
			// �R���C�_�[�̈ʒu��\��
			Vector3 pos1 = centerPos + (positionOffset * rotation);
			Vector3 pos2 = centerPos + ((positionOffset + pos2offset) * rotation);

			DrawCapsule3D(pos1, pos2, radius, 4, GetColor(0, 255, 0), GetColor(0, 0, 0), false);
		}
	}
#endif

	const float GetRadius() const override { return radius; }
};