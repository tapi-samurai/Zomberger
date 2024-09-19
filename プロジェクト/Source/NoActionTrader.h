#pragma once
#include "Actor3D.h"
#include "Quaternion.h"

class NoActionTrader : public Actor3D
{
private:
	// アニメーション設定
	enum class Anime
	{
		Idle,

		Length,
	};

	const char* AnimeName[static_cast<int>(Anime::Length)] =
	{
		"Idle",
	};

	const Animation3D AnimeData[static_cast<int>(Anime::Length)] =
	{
		Animation3D("Resource/model/player/player_idle.mv1"),
	};

public:
	static constexpr Vector3 Position = Vector3(20, 0, -10);
	static constexpr float RotationAngleY = 100.0f;
	static constexpr Vector3 Scale = Vector3(0.15f, 0.15f, 0.15f);
	static constexpr float ColliderRadius = 4.0f;
	static constexpr float InteractRadius = 9.0f;
	static constexpr Vector3 ColliderPos2Offset = Vector3(0, 2, 0);

	NoActionTrader();
};