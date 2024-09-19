#pragma once
#include "Transform3D.h"
#include "Collision3D.h"

class SphereCollider3D;
class CapsuleCollider3D;
class QuadCollider3D;

// 実際の当たり判定の処理を行う
class Collider3D
{
public:
	Vector3 positionOffset;	// 中心座標からのズレ

	bool isEnabled;		// コリジョンが有効か

	// コンストラクタ
	Collider3D(const Vector3& positionOffset) :
		positionOffset(positionOffset),
		isEnabled(true) {}

	// 衝突判定
	// ダブルディスパッチによる形状識別
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const SphereCollider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const CapsuleCollider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const QuadCollider3D* collider2) const = 0;

#ifdef _DEBUG
	// 当たり判定の表示
	virtual void DebugDraw(const Vector3& centerPos, const Quaternion& rotation) const = 0; 
#endif

	// 半径の取得
	virtual const float GetRadius() const { return 0; }

	// 横幅を取得
	virtual const float GetWidth() const { return 0; }
};