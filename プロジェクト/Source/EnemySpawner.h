#pragma once
#include "Node.h"
#include "TimeProgressManager.h"
#include "Vector3.h"
#include <vector>

class PlayerController;

class EnemySpawner : public Node
{
private:
	// ゾンビのスポーン確率の基準(日数x値+時間帯x値)
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