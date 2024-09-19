#pragma once
#include "Collider3D.h"

class QuadCollider3D : public Collider3D
{
public:
	float height;
	float width;

	// コンストラクタ
	QuadCollider3D(const Vector3& positionOffset = Vector3(), float height = 1, float width = 1) :
		Collider3D(positionOffset),
		height(height),
		width(width) {}

	// 衝突判定
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const override
	{
		// ダブルディスパッチによる形状判定
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const override
	{
		// 球×クアッド
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const override
	{
		// カプセル×クアッド
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const override
	{
		// クアッド×クアッド
		return Collision3D::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	//デバッグ時に当たり判定を描画
	void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const
	{
		// 当たり判定が有効である場合のみ表示
		if (isEnabled)
		{
			// 四角形を二つの三角形で描画する
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