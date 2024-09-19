#pragma once
#include "Collider3D.h"
#include "Vector3.h"
#include "DxLib.h"

class SphereCollider3D : public Collider3D
{
private:
	float radius;	// 球の半径

public:
	// コンストラクタ
	SphereCollider3D(const Vector3& positionOffset = Vector3(), float radius = 1) :
		Collider3D(positionOffset),
		radius(radius) {}

	// 衝突判定
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const override
	{
		// ダブルディスパッチによる形状判定
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const override
	{
		// 球×球
		return Collision3D::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const override
	{
		// 球×カプセル
		return Collision3D::Check(transform1, this, transform2, collider2);
	}
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const override
	{
		// 球×クアッド
		return Collision3D::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	// 当たり判定の描画
	void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const
	{
		// 当たり判定が有効である場合のみ表示
		if (isEnabled)
		{
			// 座標を計算
			Vector3 pos = centerPos + (positionOffset * rotation);

			// コライダーの位置を表示
			DrawSphere3D(pos, radius, 4, GetColor(0, 255, 0), GetColor(0, 0, 0), false);
		}
	}
#endif

	// 半径の取得
	const float GetRadius() const override { return radius; }
};