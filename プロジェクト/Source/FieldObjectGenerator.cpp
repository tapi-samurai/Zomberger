#include "FieldObjectGenerator.h"
#include "EnemySpawner.h"
#include "Math.h"
#include "Vector3.h"
#include "SceneField.h"
#include "ItemObject.h"
#include "ItemData.h"
#include "NoActionObject.h"
#include "Time.h"
#include "ModelData.h"
#include "StageData.h"
#include "PlayerController.h"
#include <fstream>
#include <sstream>

FieldObjectGenerator::FieldObjectGenerator(
	PlayerController* playerController, EnemySpawner* enemySpawner,
	UiInteractItemGage* const uiInteractItemGage, int currentStageNum
) :
	m_playerController(playerController),
	m_enemySpawner(enemySpawner),
	m_uiInteractItemGage(uiInteractItemGage),
	m_elapsedTime(0.0f),
	m_currentStageNum(currentStageNum)
{
	// �X�|�[���p�̔z����`
	m_spawnObjectArray =
		std::vector<std::vector<int>>(StageGridHeight, std::vector<int>(StageGridWidth, static_cast<int>(Identifier::Null)));

	// csv�t�@�C������}�b�v�f�[�^��ǂݍ���
	int enemyCount = 0;
	enemyCount = FileLoad();

	// ���I�p�̔z����`
	m_randomSelectArray =
		std::vector<std::vector<int*>>(static_cast<int>(Identifier::Length), std::vector<int*>(0));

	// �G�̃X�|�[�����W�����z����`
	m_enemySpawnPosArray = std::vector<Vector3>(enemyCount, Vector3());

	// ����̓G�̃X�|�[���܂ł̃C���^�[�o����ݒ�
	m_nextSpawnInterval = MinSpawnInterval + (rand() % (MaxSpawnInterval - MinSpawnInterval + 1));
}

void FieldObjectGenerator::Load()
{
	Node::Load();

	// �e�I�u�W�F�N�g�̌��n���𓯎��ɃJ�E���g
	for (int height = 0; height < StageGridHeight; height++)
	{
		for (int width = 0; width < StageGridWidth; width++)
		{
			// ���g��Null��������X�L�b�v
			if (m_spawnObjectArray[height][width] == static_cast<int>(Identifier::Null)) continue;

			// �ǂݍ��񂾔z��̃A�h���X�������_���I��p�̔z��ɃR�s�[
			m_randomSelectArray[m_spawnObjectArray[height][width]].push_back(&m_spawnObjectArray[height][width]);

			// �ǂݍ��񂾔z�񂪓G�łȂ����Null������
			if (m_spawnObjectArray[height][width] == static_cast<int>(Identifier::Enemy)) continue;

			m_spawnObjectArray[height][width] = static_cast<int>(Identifier::Null);
		}
	}

	// �R�s�[�������̔z�񂩂烉���_���ŃA�h���X��I�����āA���̃A�h���X�̐�����ɖ߂�
	for (int objectType = 0; objectType < static_cast<int>(Identifier::Length); objectType++)
	{
		// �G�̔z�u�͍폜���ǉ������Ȃ��Ă悢�̂ŃX�L�b�v
		if (objectType == static_cast<int>(Identifier::Enemy)) continue;

		// �I�u�W�F�N�g�̍ő�z�u�����烉���_���Ŕz�u����ݒ�
		int setObjectNum = Math::Clamp(rand() % MaxSetObjectNum[objectType], MinSetObjectNum[objectType], MaxSetObjectNum[objectType]);
		
		for (int currentSetObjectNum = 0; currentSetObjectNum < setObjectNum; currentSetObjectNum++)
		{
			// ���I�Ώۂ�������ΏI��
			if (m_randomSelectArray[objectType].size() <= 0) break;

			// �z�u�ꏊ�𒊑I
			int selectNum = rand() % m_randomSelectArray[objectType].size();

			// �z�u�ꏊ�̃A�h���X���ɃI�u�W�F�N�g�̔ԍ�����
			*m_randomSelectArray[objectType][selectNum] = objectType;

			// ����I�������ԍ��𒊑I�Ώۂ���O��
			m_randomSelectArray[objectType].erase(m_randomSelectArray[objectType].begin() + selectNum);
		}
	}

	// �e�I�u�W�F�N�g�𐶐�
	int enemyCount = 0;
	int objectNum = 0;
	for (int height = 0; height < StageGridHeight; height++)
	{
		for (int width = 0; width < StageGridWidth; width++)
		{
			// ���g��Null�ł���΃X�L�b�v
			if (m_spawnObjectArray[height][width] == static_cast<int>(Identifier::Null)) continue;

			// �z�u���W���v�Z
			float gridHeightLength = SceneField::FloorHeight / StageGridHeight;
			float gridWidthLength = SceneField::FloorWidth / StageGridWidth;

			Vector3 position =
				Vector3((-StageGridWidth / 2) * gridWidthLength, 0, (StageGridHeight / 2) * gridHeightLength) +
				Vector3(width * gridWidthLength, 0, -height * gridHeightLength
				);

			int itemNum = 0;
			switch (m_spawnObjectArray[height][width])
			{
			case static_cast<int>(Identifier::Object):
				// �����_���ŃI�u�W�F�N�g�𐶐�
				objectNum = rand() % static_cast<int>(ModelData::ObjectName::Length);
				GetParent()->AddChild(new NoActionObject(
					position, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(static_cast<float>(rand() % 360))),
					ModelData::Obstacles[objectNum].BaseScale, "Object",
					ModelData::Obstacles[objectNum].ModelName, ObjectColliderRadius
				));
				break;

			case static_cast<int>(Identifier::Stash):
				// �A�C�e���ԍ��������_���Őݒ�
				// 50%�͂��̃}�b�v�ŏo�₷���A�C�e�����o��悤�ɂ���
				itemNum = (rand() % (static_cast<int>(ItemData::RecipeItem::Length) - 1)) + 1;
				itemNum = rand() % 2 == 0 ? static_cast<int>(StageData::List[m_currentStageNum].FrequentItemName) : itemNum;

				// ���[�g�m�[�h�ɐ���
				GetParent()->AddChild(new ItemObject(m_playerController, m_uiInteractItemGage, position, Quaternion::CreateIdentity(), "-", itemNum));
				break;

			case static_cast<int>(Identifier::Enemy):
				// ���W��o�^
				m_enemySpawnPosArray[enemyCount] = position;
				enemyCount++;
				break;

			default:
				break;
			}
		}
	}

	// �����z�u�̓G�̐���ݒ�
	int spawnEnemyNum =
		(rand() % (MaxSetObjectNum[static_cast<int>(Identifier::Enemy)] - MinSetObjectNum[static_cast<int>(Identifier::Enemy)] + 1)) +
		MinSetObjectNum[static_cast<int>(Identifier::Enemy)];

	// �z�u������W�̔z��ԍ������z����`
	std::vector<int> spawnNumArray = std::vector<int>(spawnEnemyNum, -1);

	// �z�u������W�̔z��ԍ�����Ȃ��悤�Ƀ����_���Ő���
	for (int arrayNum = 0; arrayNum < spawnEnemyNum; arrayNum++)
	{
		int num = rand() % m_enemySpawnPosArray.size();

		// ���g������Ă���΂�蒼��
		if (std::find(spawnNumArray.begin(), spawnNumArray.end(), num) != spawnNumArray.end())
		{
			arrayNum--;
			continue;
		}

		spawnNumArray[arrayNum] = num;
	}

	// �����z�u�̓G�𐶐�
	for (int arrayNum = 0; arrayNum < spawnEnemyNum; arrayNum++)
	{
		m_enemySpawner->InitialSpawn(m_enemySpawnPosArray[spawnNumArray[arrayNum]]);
	}
}

void FieldObjectGenerator::Update()
{
	Node::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	if (m_elapsedTime > m_nextSpawnInterval)
	{
		// ���W����₩�烉���_���Ŏ擾
		Vector3 position = m_enemySpawnPosArray[rand() % m_enemySpawnPosArray.size()];

		// �X�|�[�����邩�𒊑I
		m_enemySpawner->DynamicSpawn(position);

		// �C���^�[�o�������Z�b�g
		m_elapsedTime = 0.0f;
		m_nextSpawnInterval = MinSpawnInterval + (rand() % (MaxSpawnInterval - MinSpawnInterval + 1));	
	}
}

void FieldObjectGenerator::Draw()
{
	Node::Draw();

#ifndef NDEBUG
	// �G�̃X�|�[�����n��\��
	for (int height = 0; height < StageGridHeight; height++)
	{
		for (int width = 0; width < StageGridWidth; width++)
		{
			if (m_spawnObjectArray[height][width] != static_cast<int>(Identifier::Enemy)) continue;

			// �z�u���W���v�Z
			int gridHeightLength = static_cast<int>(SceneField::FloorHeight / StageGridHeight);
			int gridWidthLength = static_cast<int>(SceneField::FloorWidth / StageGridWidth);
			
			Vector3 position =
				Vector3((-StageGridWidth / 2) * gridWidthLength, 0, (StageGridHeight / 2) * gridHeightLength) +
				Vector3(width * gridWidthLength, 0, -height * gridHeightLength
				);

			DrawSphere3D(position, 3, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
		}
	}
#endif

}

int FieldObjectGenerator::FileLoad()
{
	// �t�@�C���̓ǂݍ���
	std::ifstream mapFile("Resource/map/mapdata.csv");

	std::string str;
	std::string strComma;

	// �G�̐����J�E���g����
	int enemyCount = 0;

	for (int heightNum = 0; heightNum < StageGridHeight; heightNum++)
	{
		// �t�@�C���f�[�^����s���擾
		std::getline(mapFile, str);

		// ���l�����o�����߂�isstream�ɕϊ�
		std::istringstream isstr(str);

		// �l���R���}��؂�Ŏ��o��
		for (int widthNum = 0; std::getline(isstr, strComma, ','); widthNum++)
		{
			// ���o�����l��int�ɕϊ����Ĕz��ɕۑ�
			int objectType = std::stoi(strComma);
			m_spawnObjectArray[heightNum][widthNum] = objectType;

			// ���o�����l���G�ł���ΐ����J�E���g
			if (objectType == static_cast<int>(Identifier::Enemy))
			{
				enemyCount++;
			}
		}
	}

	return enemyCount;
}