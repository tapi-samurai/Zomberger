#pragma once
#include "Actor3D.h"

class PlayerController;

class PlayerInteractLandmark : public Actor3D
{
private:
	static constexpr float ColliderRadius = 3.0f;

	const PlayerController* m_playerController;

protected:
	void Update() override;

public:
	PlayerInteractLandmark(const PlayerController* playerController);
};