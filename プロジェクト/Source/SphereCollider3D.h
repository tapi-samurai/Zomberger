#pragma once
#include "Collider3D.h"
#include "Vector3.h"
#include "DxLib.h"

class SphereCollider3D : public Collider3D
{
private:
	float radius;	// ���̔��a

public:
	// �R���X�g���N�^
	SphereCollider3D(const Vector3& positionOffset = Vector3(), float radius = 1) :
		Collider3D(positionOffset),
		radius(radius) {}

	// �Փ˔���
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const override
	{
		// �_�u���f�B�X�p�b�`�ɂ��`�󔻒�
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const override
	{
		// ���~��
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const override
	{
		// ���~�J�v�Z��
		return Collision3D::Check(transform1, this, transform2, collider2);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const override
	{
		// ���~�N�A�b�h
		return Collision3D::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	// �����蔻��̕`��
	void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const
	{
		// �����蔻�肪�L���ł���ꍇ�̂ݕ\��
		if (isEnabled)
		{
			// ���W���v�Z
			Vector3 pos = centerPos + (positionOffset * rotation);

			// �R���C�_�[�̈ʒu��\��
			DrawSphere3D(pos, radius, 4, GetColor(0, 255, 0), GetColor(0, 0, 0), false);
		}
	}
#endif

	// ���a�̎擾
	const float GetRadius() const override { return radius; }
};