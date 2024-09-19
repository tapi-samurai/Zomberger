#include "Collision3D.h"
#include "DxLib.h"
#include "Actor3D.h"
#include "Vector3.h"
#include "Transform3D.h"
#include "Quaternion.h"

#include "SphereCollider3D.h"
#include "CapsuleCollider3D.h"
#include "QuadCollider3D.h"

// ���~���̓����蔻��
bool Collision3D::Check(
	const Transform3D& transform1, const SphereCollider3D* sphere1,
	const Transform3D& transform2, const SphereCollider3D* sphere2)
{
	// �v�Z�ɕK�v�Ȋe�����擾
	Vector3 positionOffset1 = sphere1->positionOffset;
	Quaternion rotation1 = transform1.rotation;
	Vector3 position1 = transform1.position + positionOffset1 * rotation1;

	Vector3 positionOffset2 = sphere2->positionOffset;
	Quaternion rotation2 = transform2.rotation;
	Vector3 position2 = transform2.position + positionOffset2 * rotation2;

	float radius1 = sphere1->GetRadius();
	float radius2 = sphere2->GetRadius();

	// �����m�̋��������a�̘a��菬������ΏՓ˂Ɣ���
	return (position1 - position2).SqrMagnitude() < (radius1 + radius2) * (radius1 + radius2) ? true : false;
}

// �J�v�Z���~�J�v�Z���̓����蔻��
bool Collision3D::Check(
	const Transform3D& transform1, const CapsuleCollider3D* capsule1,
	const Transform3D& transform2, const CapsuleCollider3D* capsule2)
{
	// �v�Z�ɕK�v�Ȋe�����擾
	Vector3 positionOffset1 = capsule1->positionOffset;
	Vector3 pos2offset1 = capsule1->pos2offset;
	Vector3 position1 = transform1.position;
	Quaternion rotation1 = transform1.rotation;

	Vector3 positionOffset2 = capsule2->positionOffset;
	Vector3 pos2offset2 = capsule2->pos2offset;
	Vector3 position2 = transform2.position;
	Quaternion rotation2 = transform2.rotation;

	// �����̗��[�̍��W���v�Z
	Vector3 startPos1 = position1 + (positionOffset1 * rotation1);
	Vector3 endPos1 = position1 + ((pos2offset1 + positionOffset1) * rotation1);

	Vector3 startPos2 = position2 + (positionOffset2 * rotation2);
	Vector3 endPos2 = position2 + ((pos2offset2 + positionOffset2) * rotation2);

	// ���J�v�Z���̔��a���擾
	float radius1 = capsule1->radius;
	float radius2 = capsule2->radius;

	// �Q�̐����̍ŋߓ_�Ԃ̋����ƂQ�̃J�v�Z���̔��a���r
	return Segment_Segment_MinLength(startPos1, endPos1, startPos2, endPos2) < radius1 + radius2 ? true : false;
}

// �N�A�b�h�~�N�A�b�h�̓����蔻��
bool Collision3D::Check(
	const Transform3D& transform1, const QuadCollider3D* quad1,
	const Transform3D& transform2, const QuadCollider3D* quad2)
{
	return false;
}

// ���~�J�v�Z���̓����蔻��
bool Collision3D::Check(
	const Transform3D& transform1, const SphereCollider3D* sphere1,
	const Transform3D& transform2, const CapsuleCollider3D* capsule2)
{
	// �v�Z�ɕK�v�Ȋe�����擾
	Vector3 positionOffset1 = sphere1->positionOffset;
	Quaternion rotation1 = transform1.rotation;
	Vector3 position1 = transform1.position + positionOffset1 * rotation1;

	Vector3 positionOffset2 = capsule2->positionOffset;
	Vector3 pos2offset2 = capsule2->pos2offset;
	Vector3 position2 = transform2.position;
	Quaternion rotation2 = transform2.rotation;

	// �J�v�Z���̗��[�̍��W���擾
	Vector3 startPos2 = position2 + (positionOffset2 * rotation2);
	Vector3 endPos2 = position2 + ((pos2offset2 + positionOffset2) * rotation2);

	// ���ƃJ�v�Z���̔��a���擾
	float radius1 = sphere1->GetRadius();
	float radius2 = capsule2->GetRadius();

	// �����Ɠ_�̍ŋߓ_�Ԃ̋����Ƌ��ƃJ�v�Z���̔��a�̘a���r
	return Segment_Point_MinLength(startPos2, endPos2, position1) < radius1 + radius2 ? true : false;
}

// ���~�N�A�b�h�̓����蔻��
bool Collision3D::Check(
	const Transform3D& transform1, const SphereCollider3D* sphere,
	const Transform3D& transform2, const QuadCollider3D* quad)
{
	// �v�Z�ɕK�v�Ȋe�����擾
	Vector3 positionOffset1 = sphere->positionOffset;
	Quaternion rotation1 = transform1.rotation;
	Vector3 position1 = transform1.position + positionOffset1 * rotation1;
	float radius = sphere->GetRadius();

	Vector3 position2 = transform2.position;
	Vector3 positionOffset2 = quad->positionOffset;
	Quaternion rotation2 = transform2.rotation;
	float height = quad->height;
	float width = quad->width;

	// �N�A�b�h�̒��_���W���v�Z
	Vector3 quadPos1 = position2 + (positionOffset2 + Vector3(-(width / 2), height / 2, 0)) * rotation2;
	Vector3 quadPos2 = position2 + (positionOffset2 + Vector3(width / 2, height / 2, 0)) * rotation2;
	Vector3 quadPos3 = position2 + (positionOffset2 + Vector3(width / 2, -(height / 2), 0)) * rotation2;
	Vector3 quadPos4 = position2 + (positionOffset2 + Vector3(-(width / 2), -(height / 2), 0)) * rotation2;

	return
		Segment_Triangle_MinLength(position1, position1, quadPos1, quadPos2, quadPos3) < radius ||
		Segment_Triangle_MinLength(position1, position1, quadPos3, quadPos4, quadPos1) < radius
		? true : false;
}

// �J�v�Z���~�N�A�b�h�̓����蔻��
bool Collision3D::Check(
	const Transform3D& transform1, const CapsuleCollider3D* capsule,
	const Transform3D& transform2, const QuadCollider3D* quad)
{
	// �v�Z�ɕK�v�Ȋe�����擾
	Vector3 positionOffset1 = capsule->positionOffset;
	Vector3 pos2offset1 = capsule->pos2offset;
	Vector3 position1 = transform1.position;
	Quaternion rotation1 = transform1.rotation;

	Vector3 position2 = transform2.position;
	Vector3 positionOffset2 = quad->positionOffset;
	Quaternion rotation2 = transform2.rotation;
	float height = quad->height;
	float width = quad->width;

	// �J�v�Z���̗��[�̍��W���擾
	Vector3 startPos = position1 + (positionOffset1 * rotation1);
	Vector3 endPos = position1 + ((pos2offset1 + positionOffset1) * rotation1);

	// �N�A�b�h�̒��_���W���擾
	Vector3 quadPos1 = position2 + (positionOffset2 + Vector3(-(width / 2), height / 2, 0)) * rotation2;
	Vector3 quadPos2 = position2 + (positionOffset2 + Vector3(width / 2, height / 2, 0)) * rotation2;
	Vector3 quadPos3 = position2 + (positionOffset2 + Vector3(width / 2, -(height / 2), 0)) * rotation2;
	Vector3 quadPos4 = position2 + (positionOffset2 + Vector3(-(width / 2), -(height / 2), 0)) * rotation2;

	return
		Segment_Triangle_MinLength(startPos, endPos, quadPos1, quadPos2, quadPos3) < capsule->radius ||
		Segment_Triangle_MinLength(startPos, endPos, quadPos3, quadPos4, quadPos1) < capsule->radius
		? true : false;
}