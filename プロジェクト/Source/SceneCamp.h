#pragma once
#include "SceneBase.h"
#include "Vector3.h"

class Node;
class PlayerController;

// キャンプシーン
class SceneCamp : public SceneBase
{
private:
	// マップ
	static constexpr float FloorHeight = 30.0f;
	static constexpr float FloorWidth = 70.0f;
	static constexpr float WallHeight = 50.0f;
	static constexpr float SceneTransitionQuadToWallLength = 2.0f;	// 床の端からシーン遷移用Quadコライダーまでの距離

	// 見えない壁(ベッドから右端の壁まで)
	static constexpr Vector3 InvisibleWall1Position = Vector3(18, WallHeight / 2, 5);
	static constexpr float InvisibleWall1AngleY = -20.0f;
	static constexpr float Wall1Width = 40.0f;

	// 見えない壁(右端の壁の手前)
	static constexpr Vector3 InvisibleWall2Position = Vector3(25, WallHeight / 2, 0);
	static constexpr float InvisibleWall2AngleY = 90.0f;
	static constexpr float Wall2Width = FloorHeight;

	// プレイヤー
	static constexpr Vector3 PlayerInitialPosition = Vector3(-25, 0, 0);
	static constexpr float PlayerInitialAngleY = -90.0f;

	// ベッド
	static constexpr Vector3 BedPosition = Vector3(-5, 0, 5);
	static constexpr float BedAngleY = -20.0f;

	// アイテムボックス
	static constexpr Vector3 ItemboxPosition = Vector3(-23, 0, -13);
	static constexpr float ItemboxAngleY = 6.0f;
	static constexpr Vector3 ItemboxScale = Vector3(0.11f, 0.11f, 0.11f);
	static constexpr float ItemboxCollisionRadius = 4;
	static constexpr float ItemboxInteractRadius = 5;

	// キッチン
	static constexpr Vector3 KitchenPosition = Vector3(-27, 0, 10);
	static constexpr float KitchenAngleY = -8.0f;
	static constexpr Vector3 KitchenScale = Vector3(6.0f, 6.0f, 6.0f);
	static constexpr float KitchenCollisionRadius = 4;
	static constexpr float KitchenInteractRadius = 5;

	// ソファ
	static constexpr Vector3 SofaPosition = Vector3(10, 0, 5);
	static constexpr float SofaAngleY = -11.0f;
	static constexpr Vector3 SofaScale = Vector3(0.1f, 0.1f, 0.1f);

	// ラジオ
	static constexpr Vector3 RadioPosition = Vector3(16, 6, 6);
	static constexpr float RadioAngleY = -10.0f;
	static constexpr Vector3 RadioScale = Vector3(0.15f, 0.15f, 0.15f);

	// コンクリ１
	static constexpr Vector3 Concrete1Position = Vector3(31, 0, 7);
	static constexpr float Concrete1AngleY = -10.0f;
	static constexpr Vector3 Concrete1Scale = Vector3(0.096f, 0.096f, 0.096f);

	// コンクリ2
	static constexpr Vector3 Concrete2Position = Vector3(29, 0, -8);
	static constexpr float Concrete2AngleY = 7.0f;
	static constexpr Vector3 Concrete2Scale = Vector3(0.08f, 0.08f, 0.08f);

	// ランプ
	static constexpr Vector3 LampPosition = Vector3(3, 0, 1);
	static constexpr float LampAngleY = -27.0f;
	static constexpr Vector3 LampScale = Vector3(0.1f, 0.1f, 0.1f);

	static constexpr float AppearanceRateAsTrader = 15.0f;	// トレーダーの出現率

	Node* m_rootNode;
	PlayerController* m_playerController;
	Vector3 m_playerPos;

	const int* m_nextStagePointer;
	int m_addTimezoneValue;
	bool m_canSpawnTrader;
	int m_seTransition;

public:
	SceneCamp(int addTimezoneValue = 1, const Vector3& playerPos = PlayerInitialPosition) :
		m_rootNode(nullptr),
		m_playerController(nullptr),
		m_playerPos(playerPos),
		m_nextStagePointer(nullptr),
		m_addTimezoneValue(addTimezoneValue),
		m_canSpawnTrader(false),
		m_seTransition(0) {}

	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual SceneBase* Update() override;
	virtual void Draw() override;
};