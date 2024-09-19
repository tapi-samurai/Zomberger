#include "EnemySpawner.h"
#include "Time.h"

#include "Zombie.h"

void EnemySpawner::InitialSpawn(const Vector3& position)
{
	// ���݂̌o�ߓ����Ǝ��ԑт��烉���_���Ȑ��l�𐶐�
	int day = TimeProgressManager::GetInstance()->GetElapsedDay();
	int timezone = TimeProgressManager::GetInstance()->GetElapsedTimezone();
	int randValue = static_cast<int>(SpawnBaseRateAsDay * day + SpawnBaseRateAsTimezone[timezone - 1]);

	// �]���r�𐶐�
	if (rand() % 100 < randValue)
	{
		AddChild(new Zombie(m_playerController, position));
	}
}

void EnemySpawner::DynamicSpawn(const Vector3& position)
{
	// �ő�X�|�[�����𒴂��Ă�����L�����Z��
	if (GetChildCount() >= MaxEnemy) return;

	// �]���r�𐶐�
	InitialSpawn(position);
}