#include "Quaternion.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Math.h"

#include <cmath>

// �f�t�H���g�R���X�g���N�^
Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

// �v�f���w�肵�ď�����
Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

// �x�N�g���ŗv�f���w��
Quaternion::Quaternion(const Vector3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0;
}

// �x�N�g���ւ̕ϊ�
Quaternion::operator Vector3() const
{
	return Vector3(x, y, z);
}

// �s��ւ̕ϊ�
Quaternion::operator MATRIX() const
{
	MATRIX m;
	float xx2 = 2 * x * x;
	float yy2 = 2 * y * y;
	float zz2 = 2 * z * z;
	float xy2 = 2 * x * y;
	float xz2 = 2 * x * z;
	float yz2 = 2 * y * z;
	float wx2 = 2 * w * x;
	float wy2 = 2 * w * y;
	float wz2 = 2 * w * z;

	// �s��ւ̐ݒ�
	m.m[0][0] = 1 - yy2 - zz2;
	m.m[0][1] = xy2 + wz2;
	m.m[0][2] = xz2 - wy2;
	m.m[0][3] = 0;
	m.m[1][0] = xy2 - wz2;
	m.m[1][1] = 1 - xx2 - zz2;
	m.m[1][2] = yz2 + wx2;
	m.m[1][3] = 0;
	m.m[2][0] = xz2 + wy2;
	m.m[2][1] = yz2 - wx2;
	m.m[2][2] = 1 - xx2 - yy2;
	m.m[2][3] = 0;
	m.m[3][0] = 0;
	m.m[3][1] = 0;
	m.m[3][2] = 0;
	m.m[3][3] = 1;

	return m;
}

// 2�̃N�H�[�^�j�I���Ԃ̊p�x��(0 �` 180)
float Quaternion::Angle(const Quaternion& from, const Quaternion& to)
{
	float angle = acos(Math::Clamp(Quaternion::Dot(from, to), -1.0f, 1.0f)) * 2;

	if (Quaternion::Dot(from, to) > 0)
	{
		return angle;
	}
	else
	{
		return Math::Fpi * 2 - angle;
	}

	return angle;
}

// ���ʐ��`���(t��0�`1)
Quaternion Quaternion::Slerp(const Quaternion &from, const Quaternion &to, float t)
{
	float theta = acos(Math::Clamp(Quaternion::Dot(from, to), -1.0f, 1.0f));
	float sinTheta = sin(theta);

	return (from * (sin((1 - t) * theta) / sinTheta) + to * (sin(t * theta) / sinTheta)).Normalize();
}

// �ő�p�x�ω����w�肷���]�̕��
Quaternion Quaternion::RotateToWards(const Quaternion& from, const Quaternion& to, float maxAngle)
{
	float angle = Quaternion::Angle(from, to);

	// float dot = Quaternion::Dot(from, to);

	// �p�x�����w�肳�ꂽ�ő�p�x�ω������������Ȃ�ڕW��Quaternion��Ԃ�
	// �[������h������
	if (angle <= maxAngle)
	{
		return to;
	}

	float t = maxAngle / angle;

	return Quaternion::Slerp(from, to, t);
}

// �C�ӎ���]�̎l�����̍쐬
Quaternion Quaternion::CreateRotationAxis(const Vector3& axis, float angle)
{
	// �P�ʃx�N�g����
	axis.Normalize();

	return Quaternion(axis.x * sin(angle / 2), axis.y * sin(angle / 2), axis.z * sin(angle / 2), cos(angle / 2));
}

// ������ύX���邽�߂̉�]�̎l�����̍쐬
// (2�̃x�N�g����180�x�̏ꍇ�͖���`)
Quaternion Quaternion::CreateRotationDirection(const Vector3& from, const Vector3& to)
{
	// ����������
	if (from / from.Magnitude() == to / to.Magnitude())
	{
		return CreateIdentity();
	}
	else
	{
		return CreateRotationAxis(Vector3::Cross(from, to), Vector3::Angle(from, to));
	}
}

// ���[�A�s�b�`�A���[�����w�肵����]�̎l�����̍쐬
Quaternion Quaternion::CreateRotationYPR(float yaw, float pitch, float roll)
{
	Quaternion qYaw(0, sin(yaw / 2), 0, cos(yaw / 2));
	Quaternion qPitch(sin(pitch / 2), 0, 0, cos(pitch / 2));
	Quaternion qRoll(0, 0, sin(roll / 2), cos(roll / 2));
	return qYaw * qPitch * qRoll;
}

Quaternion Quaternion::CreateRotationYPR(const Vector3& rotation)
{
	return CreateRotationYPR(rotation.y, rotation.x, rotation.z);
}

// �w��̍��W��������]�𐶐�
Quaternion Quaternion::CreateLookAt(const Vector3& targetPos, const Vector3& ownerPos)
{
	Vector3 axis = Vector3::Cross(-Vector3::Forward(), (targetPos - ownerPos).Normalize()).Normalize();
	float angle = Vector3::Angle(-Vector3::Forward(), (targetPos - ownerPos).Normalize());
	angle = Vector3::Dot(-Vector3::Forward(), (targetPos - ownerPos).Normalize());
	angle = std::acos(angle);

	return Quaternion::CreateRotationAxis(axis, angle);
}

// �C�ӎ���]�̎l�����̐ݒ�
void Quaternion::SetRotationAxis(const Vector3& axis, float angle)
{
	*this = CreateRotationAxis(axis, angle);
}

// ����I�u�W�F�N�g�̉�]�̉e�����󂯂��I�u�W�F�N�g�̉�]�̐���
Quaternion Quaternion::CreateRotationAxis2(const Quaternion& parentRot, const Vector3& rotation)
{
	Vector3 right = Vector3(1, 0, 0) * parentRot;
	Vector3 up = Vector3(0, 1, 0) * parentRot;
	Vector3 forward = Vector3(0, 0, 1) * parentRot;

	Quaternion rot =
		Quaternion::CreateRotationAxis(right, rotation.x) *
		Quaternion::CreateRotationAxis(up, rotation.y) *
		Quaternion::CreateRotationAxis(forward, rotation.z);

	return parentRot * rot;
}

// ������ύX���邽�߂̉�]�̎l�����̐ݒ�
// (2�̃x�N�g����180�x�̏ꍇ�͖���`)
void Quaternion::SetRotationDirection(const Vector3& from, const Vector3& to)
{
	*this = CreateRotationDirection(from, to);
}

// �l�������m�̐�
Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
	Vector3 v1 = static_cast<Vector3>(q1);
	Vector3 v2 = static_cast<Vector3>(q2);
	Vector3 xyz = q1.w * v2 + q2.w * v1 + Vector3::Cross(v2, v1);
	return Quaternion(xyz.x, xyz.y, xyz.z, q1.w * q2.w - Vector3::Dot(v2, v1));
}

// �x�N�g���ɉ�]��K�p
Vector3 operator*(const Vector3&v, const Quaternion &q)
{
	return static_cast<Vector3>(q.Conjugate() * Quaternion(v) * q);
}
