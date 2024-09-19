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

	// コンストラクタ
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Vector3&v);

	// キャスト演算子
	operator Vector3() const;	// ベクトルへの変換
	operator MATRIX() const;	// 行列への変換

	// 共役四元数を返す
	Quaternion Conjugate() const;

	// 逆元を返す
	Quaternion Inverse() const;

	// 長さの２乗を返す
	float LengthSquared() const;

	// 長さを返す
	float Length() const;

	// 四元数を正規化する
	Quaternion Normalize();

	// 内積を求める
	static float Dot(const Quaternion &q1, const Quaternion &q2);

	// 2つのクォータニオン間の角度差
	static float Angle(const Quaternion& from, const Quaternion& to);

	// 指定の座標を向く回転を生成

	// 球面線形補間を行う(tは0～1)
	static Quaternion Slerp(const Quaternion &from, const Quaternion &to, float t);

	// 最大角度変化を指定する回転の補間
	static Quaternion RotateToWards(const Quaternion& from, const Quaternion& to, float t);

	// あるオブジェクトの回転の影響を受けたオブジェクトの回転の生成
	static Quaternion CreateRotationAxis2(const Quaternion& parentRot, const Vector3& rotation);

	// 四元数の作成
	static Quaternion CreateIdentity();														// 単位四元数の作成
	static Quaternion CreateRotationAxis(const Vector3& axis, float angle);					// 任意軸回転の四元数の作成
	static Quaternion CreateRotationDirection(const Vector3& from, const Vector3& to);		// 向きを変更するための回転の四元数の作成(2つのベクトルが180度の場合は未定義)
	static Quaternion CreateRotationYPR(float yaw, float pitch, float roll);				// ヨー、ピッチ、ロールを指定した回転の四元数の作成
	static Quaternion CreateRotationYPR(const Vector3& rotation);
	static Quaternion CreateLookAt(const Vector3& targetPos, const Vector3& ownerPos);

	// 四元数の設定
	void SetIdentity();													// 単位四元数の設定
	void SetRotationAxis(const Vector3& axis, float angle);				// 任意軸回転の四元数の設定
	void SetRotationDirection(const Vector3& from, const Vector3& to);	// 向きを変更するための回転の四元数の設定(2つのベクトルが180度の場合は未定義)
	void SetRotationYPR(float yaw, float pitch, float roll);			// ヨー、ピッチ、ロールを指定した回転の四元数の設定
};

// 四元数同士の積
Quaternion operator*(const Quaternion &q1, const Quaternion &q2);

// ベクトルに回転を適用
Vector3 operator*(const Vector3&v, const Quaternion &q);

// 四元数同士の和
inline Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

// 四元数のスカラー倍
inline Quaternion operator*(const Quaternion &q, float f)
{
	return Quaternion(q.x * f, q.y * f, q.z * f, q.w * f);
}

// 共役四元数を返す
inline Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

// 逆元を返す　
inline Quaternion Quaternion::Inverse() const
{
	float normSquared = LengthSquared();

	// ゼロで割ってしまう場合は初期値を返す
	if (normSquared == 0.0f)
	{
		return Quaternion::CreateIdentity();
	}

	float invNormSquared = 1.0f / normSquared;
	return Conjugate() * invNormSquared;
}

// 長さの２乗を返す
inline float Quaternion::LengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

// 長さを返す
inline float Quaternion::Length() const
{
	return sqrt(LengthSquared());
}

// 正規化する
inline Quaternion Quaternion::Normalize()
{
	x /= Length();
	y /= Length();
	z /= Length();
	w /= Length();

	return *this;
}

// 2つの四元数の内積を求める
inline float Quaternion::Dot(const Quaternion &q1, const Quaternion &q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

// 単位四元数の作成
inline Quaternion Quaternion::CreateIdentity()
{
	return Quaternion(0, 0, 0, 1);
}

// 単位四元数の設定
inline void Quaternion::SetIdentity()
{
	*this = CreateIdentity();
}

// ヨー、ピッチ、ロールを指定した回転の四元数の設定
inline void Quaternion::SetRotationYPR(float yaw, float pitch, float roll)
{
	*this = CreateRotationYPR(yaw, pitch, roll);
}