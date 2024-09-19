#pragma once
#include "Vector3.h"
#include <vector>

namespace CameraData
{
	struct CameraTransform
	{
		const Vector3 CameraPosition;
		const float CameraAngleX;
		const float CameraAngleY;
		const float CameraAngleZ;

		constexpr CameraTransform(const Vector3 position, float angleX = 0, float angleY = 0, float angleZ = 0) :
			CameraPosition(position),
			CameraAngleX(angleX),
			CameraAngleY(angleY),
			CameraAngleZ(angleZ) {}
	};

	static const std::vector<CameraTransform> Camp =
	{
		CameraTransform(Vector3(-30, 30, -30), 30.0f, 35.0f, 0),
		CameraTransform(Vector3(0, 40, -40), 35.0f, 0, 0),
		CameraTransform(Vector3(36, 33, -30), 30.0f, -55.0f, 0),
	};

	static const std::vector<CameraTransform> Field =
	{
		CameraTransform(Vector3(40, 40, -40), 30.0f, -45.0f, 0),
		CameraTransform(Vector3(37, 55, -19), 52.0f, -65.0f, 0),
		CameraTransform(Vector3(25, 19, -44), 15.0f, -35.0f, 0),
	};
}