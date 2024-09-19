#pragma once
#include "Node.h"
#include "Vector3.h"
#include <vector>

class EnemySpawner;
class PlayerController;
class UiInteractItemGage;

class FieldObjectGenerator : public Node
{
public:
	// �t�@�C���̒��g�����ʂ��邽�߂̗񋓑�
	enum class Identifier
	{
		Object,
		Stash,
		Enemy,

		Length,

		Null,
	};

private:
	static constexpr int StageGridHeight = 9;
	static constexpr int StageGridWidth = 9;
	static constexpr int MaxSpawnInterval = 20;	// ���Ԍo�߂œG���X�|�[������Œ��C���^�[�o��
	static constexpr int MinSpawnInterval = 10;	// ���Ԍo�߂œG���X�|�[������ŒZ�C���^�[�o��
	static constexpr float ObjectColliderRadius = 3.0f;

	static constexpr int MaxSetObjectNum[static_cast<int>(Identifier::Length)] = { 34, 3, 3 };	// �I�u�W�F�N�g�̍ő��z�u��
	static constexpr int MinSetObjectNum[static_cast<int>(Identifier::Length)] = { 10, 1, 1 };	// �I�u�W�F�N�g�̍ŏ��z�u��

	std::vector<std::vector<int>> m_spawnObjectArray;	// ��������I�u�W�F�N�g�̔z��
	std::vector<std::vector<int*>> m_randomSelectArray;	// �I�u�W�F�N�g�̐����ꏊ�̒��I�p�z��
	std::vector<Vector3> m_enemySpawnPosArray;				// �G�̃X�|�[�����W�����z��

	EnemySpawner* m_enemySpawner;
	PlayerController* m_playerController;
	UiInteractItemGage* const m_uiInteractItemGage;

	int m_nextSpawnInterval;
	int m_currentStageNum;

	float m_elapsedTime;

	int FileLoad();

protected:
	void Load() override;
	void Update() override;
	void Draw() override;

public:
	FieldObjectGenerator(
		PlayerController* playerController, EnemySpawner* enemySpawner,
		UiInteractItemGage* const uiInteractItemGage, int currentStageNum
	);
};