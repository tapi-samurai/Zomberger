#pragma once

class Transform3D;
class SphereCollider3D;
class CapsuleCollider3D;
class QuadCollider3D;

class Collision3D
{
public:
	static bool Check(
		const Transform3D& transform1, const SphereCollider3D* sphere1,
		const Transform3D& transform2, const SphereCollider3D* sphere2);

	static bool Check(
		const Transform3D& transform1, const CapsuleCollider3D* capsule1,
		const Transform3D& transform2, const CapsuleCollider3D* capsule2);

	static bool Check(
		const Transform3D& transform1, const QuadCollider3D* quad1,
		const Transform3D& transform2, const QuadCollider3D* quad2);

	static bool Check(
		const Transform3D& transform1, const SphereCollider3D* sphere,
		const Transform3D& transform2, const CapsuleCollider3D* capsule);

	static bool Check(
		const Transform3D& transform1, const SphereCollider3D* sphere,
		const Transform3D& transform2, const QuadCollider3D* quad);

	static bool Check(
		const Transform3D& transform1, const CapsuleCollider3D* capsule,
		const Transform3D& transform2, const QuadCollider3D* quad);
};