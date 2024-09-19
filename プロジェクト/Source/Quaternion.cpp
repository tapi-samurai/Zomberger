#include "Quaternion.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Math.h"

#include <cmath>

// デフォルトコンストラクタ
Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

// 要素を指定して初期化
Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

// ベクトルで要素を指定
Quaternion::Quaternion(const Vector3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0;
}

// ベクトルへの変換
Quaternion::operator Vector3() const
{
	return Vector3(x, y, z);
}

// 行列への変換
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

	// 行列への設定
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

// 2つのクォータニオン間の角度差(0 ～ 180)
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

// 球面線形補間(tは0～1)
Quaternion Quaternion::Slerp(const Quaternion &from, const Quaternion &to, float t)
{
	float theta = acos(Math::Clamp(Quaternion::Dot(from, to), -1.0f, 1.0f));
	float sinTheta = sin(theta);

	return (from * (sin((1 - t) * theta) / sinTheta) + to * (sin(t * theta) / sinTheta)).Normalize();
}

// 最大角度変化を指定する回転の補間
Quaternion Quaternion::RotateToWards(const Quaternion& from, const Quaternion& to, float maxAngle)
{
	float angle = Quaternion::Angle(from, to);

	// float dot = Quaternion::Dot(from, to);

	// 角度差が指定された最大角度変化よりも小さいなら目標のQuaternionを返す
	// ゼロ割を防ぐ処理
	if (angle <= maxAngle)
	{
		return to;
	}

	float t = maxAngle / angle;

	return Quaternion::Slerp(from, to, t);
}

// 任意軸回転の四元数の作成
Quaternion Quaternion::CreateRotationAxis(const Vector3& axis, float angle)
{
	// 単位ベクトル化
	axis.Normalize();

	return Quaternion(axis.x * sin(angle / 2), axis.y * sin(angle / 2), axis.z * sin(angle / 2), cos(angle / 2));
}

// 向きを変更するための回転の四元数の作成
// (2つのベクトルが180度の場合は未定義)
Quaternion Quaternion::CreateRotationDirection(const Vector3& from, const Vector3& to)
{
	// 向きが同じ
	if (from / from.Magnitude() == to / to.Magnitude())
	{
		return CreateIdentity();
	}
	else
	{
		return CreateRotationAxis(Vector3::Cross(from, to), Vector3::Angle(from, to));
	}
}

// ヨー、ピッチ、ロールを指定した回転の四元数の作成
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

// 指定の座標を向く回転を生成
Quaternion Quaternion::CreateLookAt(const Vector3& targetPos, const Vector3& ownerPos)
{
	Vector3 axis = Vector3::Cross(-Vector3::Forward(), (targetPos - ownerPos).Normalize()).Normalize();
	float angle = Vector3::Angle(-Vector3::Forward(), (targetPos - ownerPos).Normalize());
	angle = Vector3::Dot(-Vector3::Forward(), (targetPos - ownerPos).Normalize());
	angle = std::acos(angle);

	return Quaternion::CreateRotationAxis(axis, angle);
}

// 任意軸回転の四元数の設定
void Quaternion::SetRotationAxis(const Vector3& axis, float angle)
{
	*this = CreateRotationAxis(axis, angle);
}

// あるオブジェクトの回転の影響を受けたオブジェクトの回転の生成
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

// 向きを変更するための回転の四元数の設定
// (2つのベクトルが180度の場合は未定義)
void Quaternion::SetRotationDirection(const Vector3& from, const Vector3& to)
{
	*this = CreateRotationDirection(from, to);
}

// 四元数同士の積
Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
	Vector3 v1 = static_cast<Vector3>(q1);
	Vector3 v2 = static_cast<Vector3>(q2);
	Vector3 xyz = q1.w * v2 + q2.w * v1 + Vector3::Cross(v2, v1);
	return Quaternion(xyz.x, xyz.y, xyz.z, q1.w * q2.w - Vector3::Dot(v2, v1));
}

// ベクトルに回転を適用
Vector3 operator*(const Vector3&v, const Quaternion &q)
{
	return static_cast<Vector3>(q.Conjugate() * Quaternion(v) * q);
}
