#include "NoActionObject.h"
#include "SphereCollider3D.h"
#include "Math.h"

NoActionObject::NoActionObject(
	const Vector3& position, const Quaternion& rotation, const Vector3& scale,
	const char* name, const char* modelName, float colliderRadius
) :
	Actor3D(name, modelName)
{
	// 座標、回転、拡縮を設定
	m_transform.position = position;
	m_transform.rotation = rotation;
	m_transform.scale = scale;

	// コライダーを設定
	m_collider = new SphereCollider3D(Vector3(0, colliderRadius, 0), colliderRadius);
}