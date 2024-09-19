#pragma once
#include "Collider3D.h"

class QuadCollider3D : public Collider3D
{
public:
	float height;
	float width;

	// �R���X�g���N�^
	QuadCollider3D(const Vector3& positionOffset = Vector3(), float height = 1, float width = 1) :
		Collider3D(positionOffset),
		height(height),
		width(width) {}

	// �Փ˔���
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const override
	{
		// �_�u���f�B�X�p�b�`�ɂ��`�󔻒�
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const override
	{
		// ���~�N�A�b�h
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const override
	{
		// �J�v�Z���~�N�A�b�h
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const override
	{
		// �N�A�b�h�~�N�A�b�h
		return Collision3D::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	//�f�o�b�O���ɓ����蔻���`��
	void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const
	{
		// �����蔻�肪�L���ł���ꍇ�̂ݕ\��
		if (isEnabled)
		{
			// �l�p�`���̎O�p�`�ŕ`�悷��
			Vector3 pos1 = centerPos + (positionOffset + Vector3(-(width / 2), height / 2, 0)) * rotation;
			Vector3 pos2 = centerPos + (positionOffset + Vector3(width / 2, height / 2, 0)) * rotation;
			Vector3 pos3 = centerPos + (positionOffset + Vector3(width / 2, -(height / 2), 0)) * rotation;
			Vector3 pos4 = centerPos + (positionOffset + Vector3(-(width / 2), -(height / 2), 0)) * rotation;

			DrawTriangle3D(pos1, pos2, pos3, GetColor(0, 255, 0), false);
			DrawTriangle3D(pos3, pos4, pos1, GetColor(0, 255, 0), false);
		}
	}
#endif

	const float GetWidth() const override { return width; }
};