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
	// ファイルの中身を識別するための列挙体
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
	static constexpr int MaxSpawnInterval = 20;	// 時間経過で敵がスポーンする最長インターバル
	static constexpr int MinSpawnInterval = 10;	// 時間経過で敵がスポーンする最短インターバル
	static constexpr float ObjectColliderRadius = 3.0f;

	static constexpr int MaxSetObjectNum[static_cast<int>(Identifier::Length)] = { 34, 3, 3 };	// オブジェクトの最多配置数
	static constexpr int MinSetObjectNum[static_cast<int>(Identifier::Length)] = { 10, 1, 1 };	// オブジェクトの最少配置数

	std::vector<std::vector<int>> m_spawnObjectArray;	// 生成するオブジェクトの配列
	std::vector<std::vector<int*>> m_randomSelectArray;	// オブジェクトの生成場所の抽選用配列
	std::vector<Vector3> m_enemySpawnPosArray;				// 敵のスポーン座標を持つ配列

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