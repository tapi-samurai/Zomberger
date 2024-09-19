#pragma once
#include "SceneBase.h"
#include "Vector3.h"

class Node;
class PlayerInteractLandmark;
class EnemySpawner;

// フィールドシーン
class SceneField : public SceneBase
{
	// マップ
public:
	static constexpr float FloorHeight = 54.0f;
	static constexpr float FloorWidth = 54.0f;
	static constexpr float WallHeight = 50.0f;
private:

	// カメラ
	static constexpr float SceneTransitionQuadToWallLength = 2.0f;	// 床の端からシーン遷移用Quadコライダーまでの距離

	// プレイヤー
	static constexpr Vector3 PlayerInitialPosition = Vector3(0, 0, -20);
	static constexpr float PlayerInitialAngleY = 180.0f;

	// 時間の遷移
	static constexpr int MaxTimezone = 5;

	static constexpr float SceneTransitionDelay = 2.0f;	// シーン遷移の条件を満たしてからシーン遷移するまでの時間

	Node* m_rootNode;
	PlayerInteractLandmark* m_playerInteractLandmark;
	EnemySpawner* m_enemySpawner;

	int m_currentStageNum;
	const int* m_nextStagePointer;
	float m_elapsedTime;
	int m_addTimezoneValue;
	int m_seTransition;

public:
	SceneField(int stageNum, int addTimezoneValue = 1) :
		m_rootNode(nullptr),
		m_playerInteractLandmark(nullptr),
		m_currentStageNum(stageNum),
		m_nextStagePointer(nullptr),
		m_enemySpawner(nullptr),
		m_elapsedTime(0),
		m_addTimezoneValue(addTimezoneValue),
		m_seTransition(0) {}

	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual SceneBase* Update() override;
	virtual void Draw() override;
};
