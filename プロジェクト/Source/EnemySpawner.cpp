#include "EnemySpawner.h"
#include "Time.h"

#include "Zombie.h"

void EnemySpawner::InitialSpawn(const Vector3& position)
{
	// 現在の経過日数と時間帯からランダムな数値を生成
	int day = TimeProgressManager::GetInstance()->GetElapsedDay();
	int timezone = TimeProgressManager::GetInstance()->GetElapsedTimezone();
	int randValue = static_cast<int>(SpawnBaseRateAsDay * day + SpawnBaseRateAsTimezone[timezone - 1]);

	// ゾンビを生成
	if (rand() % 100 < randValue)
	{
		AddChild(new Zombie(m_playerController, position));
	}
}

void EnemySpawner::DynamicSpawn(const Vector3& position)
{
	// 最大スポーン数を超えていたらキャンセル
	if (GetChildCount() >= MaxEnemy) return;

	// ゾンビを生成
	InitialSpawn(position);
}