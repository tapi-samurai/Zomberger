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
	// スポーン用の配列を定義
	m_spawnObjectArray =
		std::vector<std::vector<int>>(StageGridHeight, std::vector<int>(StageGridWidth, static_cast<int>(Identifier::Null)));

	// csvファイルからマップデータを読み込む
	int enemyCount = 0;
	enemyCount = FileLoad();

	// 抽選用の配列を定義
	m_randomSelectArray =
		std::vector<std::vector<int*>>(static_cast<int>(Identifier::Length), std::vector<int*>(0));

	// 敵のスポーン座標を持つ配列を定義
	m_enemySpawnPosArray = std::vector<Vector3>(enemyCount, Vector3());

	// 次回の敵のスポーンまでのインターバルを設定
	m_nextSpawnInterval = MinSpawnInterval + (rand() % (MaxSpawnInterval - MinSpawnInterval + 1));
}

void FieldObjectGenerator::Load()
{
	Node::Load();

	// 各オブジェクトの候補地数を同時にカウント
	for (int height = 0; height < StageGridHeight; height++)
	{
		for (int width = 0; width < StageGridWidth; width++)
		{
			// 中身がNullだったらスキップ
			if (m_spawnObjectArray[height][width] == static_cast<int>(Identifier::Null)) continue;

			// 読み込んだ配列のアドレスをランダム選択用の配列にコピー
			m_randomSelectArray[m_spawnObjectArray[height][width]].push_back(&m_spawnObjectArray[height][width]);

			// 読み込んだ配列が敵でなければNullを入れる
			if (m_spawnObjectArray[height][width] == static_cast<int>(Identifier::Enemy)) continue;

			m_spawnObjectArray[height][width] = static_cast<int>(Identifier::Null);
		}
	}

	// コピーした方の配列からランダムでアドレスを選択して、そのアドレスの先を元に戻す
	for (int objectType = 0; objectType < static_cast<int>(Identifier::Length); objectType++)
	{
		// 敵の配置は削除も追加もしなくてよいのでスキップ
		if (objectType == static_cast<int>(Identifier::Enemy)) continue;

		// オブジェクトの最大配置数からランダムで配置数を設定
		int setObjectNum = Math::Clamp(rand() % MaxSetObjectNum[objectType], MinSetObjectNum[objectType], MaxSetObjectNum[objectType]);
		
		for (int currentSetObjectNum = 0; currentSetObjectNum < setObjectNum; currentSetObjectNum++)
		{
			// 抽選対象が無ければ終了
			if (m_randomSelectArray[objectType].size() <= 0) break;

			// 配置場所を抽選
			int selectNum = rand() % m_randomSelectArray[objectType].size();

			// 配置場所のアドレス宛にオブジェクトの番号を代入
			*m_randomSelectArray[objectType][selectNum] = objectType;

			// 今回選択した番号を抽選対象から外す
			m_randomSelectArray[objectType].erase(m_randomSelectArray[objectType].begin() + selectNum);
		}
	}

	// 各オブジェクトを生成
	int enemyCount = 0;
	int objectNum = 0;
	for (int height = 0; height < StageGridHeight; height++)
	{
		for (int width = 0; width < StageGridWidth; width++)
		{
			// 中身がNullであればスキップ
			if (m_spawnObjectArray[height][width] == static_cast<int>(Identifier::Null)) continue;

			// 配置座標を計算
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
				// ランダムでオブジェクトを生成
				objectNum = rand() % static_cast<int>(ModelData::ObjectName::Length);
				GetParent()->AddChild(new NoActionObject(
					position, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(static_cast<float>(rand() % 360))),
					ModelData::Obstacles[objectNum].BaseScale, "Object",
					ModelData::Obstacles[objectNum].ModelName, ObjectColliderRadius
				));
				break;

			case static_cast<int>(Identifier::Stash):
				// アイテム番号をランダムで設定
				// 50%はそのマップで出やすいアイテムが出るようにする
				itemNum = (rand() % (static_cast<int>(ItemData::RecipeItem::Length) - 1)) + 1;
				itemNum = rand() % 2 == 0 ? static_cast<int>(StageData::List[m_currentStageNum].FrequentItemName) : itemNum;

				// ルートノードに生成
				GetParent()->AddChild(new ItemObject(m_playerController, m_uiInteractItemGage, position, Quaternion::CreateIdentity(), "-", itemNum));
				break;

			case static_cast<int>(Identifier::Enemy):
				// 座標を登録
				m_enemySpawnPosArray[enemyCount] = position;
				enemyCount++;
				break;

			default:
				break;
			}
		}
	}

	// 初期配置の敵の数を設定
	int spawnEnemyNum =
		(rand() % (MaxSetObjectNum[static_cast<int>(Identifier::Enemy)] - MinSetObjectNum[static_cast<int>(Identifier::Enemy)] + 1)) +
		MinSetObjectNum[static_cast<int>(Identifier::Enemy)];

	// 配置する座標の配列番号を持つ配列を定義
	std::vector<int> spawnNumArray = std::vector<int>(spawnEnemyNum, -1);

	// 配置する座標の配列番号を被らないようにランダムで生成
	for (int arrayNum = 0; arrayNum < spawnEnemyNum; arrayNum++)
	{
		int num = rand() % m_enemySpawnPosArray.size();

		// 中身が被っていればやり直し
		if (std::find(spawnNumArray.begin(), spawnNumArray.end(), num) != spawnNumArray.end())
		{
			arrayNum--;
			continue;
		}

		spawnNumArray[arrayNum] = num;
	}

	// 初期配置の敵を生成
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
		// 座標を候補からランダムで取得
		Vector3 position = m_enemySpawnPosArray[rand() % m_enemySpawnPosArray.size()];

		// スポーンするかを抽選
		m_enemySpawner->DynamicSpawn(position);

		// インターバルをリセット
		m_elapsedTime = 0.0f;
		m_nextSpawnInterval = MinSpawnInterval + (rand() % (MaxSpawnInterval - MinSpawnInterval + 1));	
	}
}

void FieldObjectGenerator::Draw()
{
	Node::Draw();

#ifndef NDEBUG
	// 敵のスポーン候補地を表示
	for (int height = 0; height < StageGridHeight; height++)
	{
		for (int width = 0; width < StageGridWidth; width++)
		{
			if (m_spawnObjectArray[height][width] != static_cast<int>(Identifier::Enemy)) continue;

			// 配置座標を計算
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
	// ファイルの読み込み
	std::ifstream mapFile("Resource/map/mapdata.csv");

	std::string str;
	std::string strComma;

	// 敵の数をカウントする
	int enemyCount = 0;

	for (int heightNum = 0; heightNum < StageGridHeight; heightNum++)
	{
		// ファイルデータを一行ずつ取得
		std::getline(mapFile, str);

		// 数値を取り出すためにisstreamに変換
		std::istringstream isstr(str);

		// 値をコンマ区切りで取り出す
		for (int widthNum = 0; std::getline(isstr, strComma, ','); widthNum++)
		{
			// 取り出した値をintに変換して配列に保存
			int objectType = std::stoi(strComma);
			m_spawnObjectArray[heightNum][widthNum] = objectType;

			// 取り出した値が敵であれば数をカウント
			if (objectType == static_cast<int>(Identifier::Enemy))
			{
				enemyCount++;
			}
		}
	}

	return enemyCount;
}