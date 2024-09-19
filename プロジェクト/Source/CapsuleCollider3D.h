#pragma once
#include "Collider3D.h"
#include "Vector3.h"
#include "DxLib.h"

class CapsuleCollider3D : public Collider3D
{
public:
	Vector3 pos2offset;	// カプセルの一点の座標オフセット
	float radius;		// 球の半径

	// コンストラクタ
	CapsuleCollider3D(const Vector3& positionOffset = Vector3(), Vector3 pos2offset = Vector3(), float radius = 1) :
		Collider3D(positionOffset),
		pos2offset(pos2offset),
		radius(radius) {}

	// 衝突判定
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const override
	{
		// ダブルディスパッチによる形状判定
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const override
	{
		// 球×カプセル
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const override
	{
		// カプセル×カプセル
		return Collision3D::Check(transform1, this, transform2, collider2);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const override
	{
		// クアッド×カプセル
		return Collision3D::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	//デバッグ時に当たり判定を描画
	void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const
	{
		// 当たり判定が有効である場合のみ表示
		if (isEnabled)
		{
			// コライダーの位置を表示
			Vector3 pos1 = centerPos + (positionOffset * rotation);
			Vector3 pos2 = centerPos + ((positionOffset + pos2offset) * rotation);

			DrawCapsule3D(pos1, pos2, radius, 4, GetColor(0, 255, 0), GetColor(0, 0, 0), false);
		}
	}
#endif

	const float GetRadius() const override { return radius; }
};