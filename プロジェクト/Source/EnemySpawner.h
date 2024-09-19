#pragma once
#include "Node.h"
#include "TimeProgressManager.h"
#include "Vector3.h"
#include <vector>

class PlayerController;

class EnemySpawner : public Node
{
private:
	// �]���r�̃X�|�[���m���̊(����x�l+���ԑ�x�l)
	static constexpr float SpawnBaseRateAsDay = 11.0f;	
	static constexpr float SpawnBaseRateAsTimezone[TimeProgressManager::MaxTimezone + 1] =
	{
		10, 20, 30, 40, 50
	};

	static constexpr int MaxEnemy = 5;

	PlayerController* m_playerController;

	float m_elapsedTime;

public:
	EnemySpawner(PlayerController* playerController) :
		m_playerController(playerController),
		m_elapsedTime(0) {}

	void InitialSpawn(const Vector3& position);

	void DynamicSpawn(const Vector3& position);
};