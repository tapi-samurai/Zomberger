#pragma once
#include <cmath>
#include "Vector3.h"
#include "DxLib.h"

struct Quaternion
{
	float w;
	float x;
	float y;
	float z;

	// �R���X�g���N�^
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Vector3&v);

	// �L���X�g���Z�q
	operator Vector3() const;	// �x�N�g���ւ̕ϊ�
	operator MATRIX() const;	// �s��ւ̕ϊ�

	// �����l������Ԃ�
	Quaternion Conjugate() const;

	// �t����Ԃ�
	Quaternion Inverse() const;

	// �����̂Q���Ԃ�
	float LengthSquared() const;

	// ������Ԃ�
	float Length() const;

	// �l�����𐳋K������
	Quaternion Normalize();

	// ���ς����߂�
	static float Dot(const Quaternion &q1, const Quaternion &q2);

	// 2�̃N�H�[�^�j�I���Ԃ̊p�x��
	static float Angle(const Quaternion& from, const Quaternion& to);

	// �w��̍��W��������]�𐶐�

	// ���ʐ��`��Ԃ��s��(t��0�`1)
	static Quaternion Slerp(const Quaternion &from, const Quaternion &to, float t);

	// �ő�p�x�ω����w�肷���]�̕��
	static Quaternion RotateToWards(const Quaternion& from, const Quaternion& to, float t);

	// ����I�u�W�F�N�g�̉�]�̉e�����󂯂��I�u�W�F�N�g�̉�]�̐���
	static Quaternion CreateRotationAxis2(const Quaternion& parentRot, const Vector3& rotation);

	// �l�����̍쐬
	static Quaternion CreateIdentity();														// �P�ʎl�����̍쐬
	static Quaternion CreateRotationAxis(const Vector3& axis, float angle);					// �C�ӎ���]�̎l�����̍쐬
	static Quaternion CreateRotationDirection(const Vector3& from, const Vector3& to);		// ������ύX���邽�߂̉�]�̎l�����̍쐬(2�̃x�N�g����180�x�̏ꍇ�͖���`)
	static Quaternion CreateRotationYPR(float yaw, float pitch, float roll);				// ���[�A�s�b�`�A���[�����w�肵����]�̎l�����̍쐬
	static Quaternion CreateRotationYPR(const Vector3& rotation);
	static Quaternion CreateLookAt(const Vector3& targetPos, const Vector3& ownerPos);

	// �l�����̐ݒ�
	void SetIdentity();													// �P�ʎl�����̐ݒ�
	void SetRotationAxis(const Vector3& axis, float angle);				// �C�ӎ���]�̎l�����̐ݒ�
	void SetRotationDirection(const Vector3& from, const Vector3& to);	// ������ύX���邽�߂̉�]�̎l�����̐ݒ�(2�̃x�N�g����180�x�̏ꍇ�͖���`)
	void SetRotationYPR(float yaw, float pitch, float roll);			// ���[�A�s�b�`�A���[�����w�肵����]�̎l�����̐ݒ�
};

// �l�������m�̐�
Quaternion operator*(const Quaternion &q1, const Quaternion &q2);

// �x�N�g���ɉ�]��K�p
Vector3 operator*(const Vector3&v, const Quaternion &q);

// �l�������m�̘a
inline Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

// �l�����̃X�J���[�{
inline Quaternion operator*(const Quaternion &q, float f)
{
	return Quaternion(q.x * f, q.y * f, q.z * f, q.w * f);
}

// �����l������Ԃ�
inline Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

// �t����Ԃ��@
inline Quaternion Quaternion::Inverse() const
{
	float normSquared = LengthSquared();

	// �[���Ŋ����Ă��܂��ꍇ�͏����l��Ԃ�
	if (normSquared == 0.0f)
	{
		return Quaternion::CreateIdentity();
	}

	float invNormSquared = 1.0f / normSquared;
	return Conjugate() * invNormSquared;
}

// �����̂Q���Ԃ�
inline float Quaternion::LengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

// ������Ԃ�
inline float Quaternion::Length() const
{
	return sqrt(LengthSquared());
}

// ���K������
inline Quaternion Quaternion::Normalize()
{
	x /= Length();
	y /= Length();
	z /= Length();
	w /= Length();

	return *this;
}

// 2�̎l�����̓��ς����߂�
inline float Quaternion::Dot(const Quaternion &q1, const Quaternion &q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

// �P�ʎl�����̍쐬
inline Quaternion Quaternion::CreateIdentity()
{
	return Quaternion(0, 0, 0, 1);
}

// �P�ʎl�����̐ݒ�
inline void Quaternion::SetIdentity()
{
	*this = CreateIdentity();
}

// ���[�A�s�b�`�A���[�����w�肵����]�̎l�����̐ݒ�
inline void Quaternion::SetRotationYPR(float yaw, float pitch, float roll)
{
	*this = CreateRotationYPR(yaw, pitch, roll);
}