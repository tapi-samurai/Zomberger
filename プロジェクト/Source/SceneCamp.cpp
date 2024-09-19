#include "SceneCamp.h"
#include "Node.h"
#include "Camera.h"
#include "Math.h"
#include "Quad3D.h"
#include "ItemField.h"
#include "SceneTransitionQuad.h"
#include "PlayerController.h"
#include "PlayerInteractLandmark.h"
#include "ModelData.h"
#include "SceneField.h"
#include "StageData.h"
#include "UiInventory.h"
#include "UiItembox.h"
#include "UiKitchen.h"
#include "SceneResult.h"
#include "UiElapsedDayTime.h"
#include "TimeProgressManager.h"
#include "NoActionObject.h"
#include "SleepBed.h"
#include "UiSwitchObject.h"
#include "PlayerState.h"
#include "UiTrader.h"
#include "PlayerScoopAttack.h"
#include "Time.h"
#include "SunLight.h"
#include "UiHealth.h"
#include "UiSatietyLevel.h"
#include "UiInteractable.h"
#include "NoActionTrader.h"
#include "ModelDisplay.h"
#include "CameraData.h"
#include "ItemField.h"
#include "ItemData.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "UiTextMessage.h"

void SceneCamp::Initialize()
{
	m_rootNode = new Node();

	// 時間帯を進める
	if (m_addTimezoneValue != 0)
	{
		TimeProgressManager::GetInstance()->AddElapsedTimezone(m_addTimezoneValue);
	}
	else
	{
		TimeProgressManager::GetInstance()->TimezoneInitialization();
	}

	// トレーダーが出現するか抽選
	m_canSpawnTrader = static_cast<int>(rand() % 100) < AppearanceRateAsTrader ? true : false;

	// インベントリ生成
	UiInventory* uiInventory = new UiInventory();

	// インタラクト可能UI
	UiInteractable* const uiInteractable = new UiInteractable();

	// テキストメッセージUI
	UiTextMessage* const uiTextMessage = new UiTextMessage();

	// カメラ
	int cameraNum = rand() % CameraData::Camp.size();
	m_rootNode->AddChild(new Camera(
		CameraData::Camp[cameraNum].CameraPosition,
		Quaternion::CreateRotationAxis(Vector3::Right(), Math::DegToRad(CameraData::Camp[cameraNum].CameraAngleX)) *
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(CameraData::Camp[cameraNum].CameraAngleY)) *
		Quaternion::CreateRotationAxis(Vector3::Forward(), Math::DegToRad(CameraData::Camp[cameraNum].CameraAngleZ)),
		1.0f, 200.0f
	));

	// 床
	m_rootNode->AddChild(new Quad3D(
		Vector3(), Quaternion::CreateRotationAxis(Vector3::Right(), Math::DegToRad(90.0f)), FloorHeight, FloorWidth,
		"Floor", StageData::List[static_cast<int>(StageData::Name::Camp)].FloorTextureName)
	);

	// 壁とシーン遷移用の当たり判定を生成
	for (int i = 0; i < 4; i++)
	{
		// 表示用の変数を用意
		float length = (i + 1) % 2 == 0 ? FloorHeight / 2 : FloorWidth / 2;
		float wallWidth = (i + 1) % 2 == 0 ? FloorWidth : FloorHeight;
		bool isDisplay = i == 1 || i == 2 ? true : false;
		Quaternion rotation = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(-90.0f + 90.0f * i));

		// 左から時計回りに壁を生成
		m_rootNode->AddChild(new Quad3D(
			Vector3::Forward() * length * rotation + Vector3(0, WallHeight / 2, 0),
			rotation, WallHeight, wallWidth,
			"Wall", StageData::List[static_cast<int>(StageData::Name::Camp)].WallTextureName, isDisplay, GetColorU8(100, 100, 100, 200)
		));

		// 最初に生成した壁の近くにのみシーン遷移用の当たり判定を持つオブジェクトを生成
		// 夜になっていたら生成しない
		if (i == 0 && TimeProgressManager::GetInstance()->GetElapsedTimezone() != TimeProgressManager::MaxTimezone)
		{
			m_rootNode->AddChild(new SceneTransitionQuad(
				uiInteractable, uiTextMessage, m_nextStagePointer, rand() % static_cast<int>(StageData::Name::Length),
				Vector3::Forward() * (length - SceneTransitionQuadToWallLength) * rotation + Vector3(0, WallHeight / 2, 0),
				rotation, WallHeight ,wallWidth
			));
		}
	}

	// 見えない壁(ベッドから右端の壁まで)
	m_rootNode->AddChild(new Quad3D(
		InvisibleWall1Position,
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(InvisibleWall1AngleY)),
		WallHeight, Wall1Width, "Wall", nullptr, false
	));

	// 見えない壁(右端の壁の手前)
	m_rootNode->AddChild(new Quad3D(
		InvisibleWall2Position,
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(InvisibleWall2AngleY)),
		WallHeight, Wall2Width, "Wall", nullptr, false
	));

	// プレイヤー
	Vector3 playerPos = m_playerPos;
	Quaternion playerRot = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(PlayerInitialAngleY));
	m_playerController = new PlayerController(playerPos, playerRot, uiInventory);
	m_rootNode->AddChild(m_playerController);

	// スコップ攻撃オブジェクト
	PlayerScoopAttack* playerScoopAttack = new PlayerScoopAttack(m_playerController);
	m_playerController->AddChild(playerScoopAttack);

	// インタラクトの検知対象オブジェクト
	m_playerController->AddChild(new PlayerInteractLandmark(m_playerController));
	
	// ベッド
	m_rootNode->AddChild(new SleepBed(
		uiInteractable, uiTextMessage, BedPosition, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(BedAngleY)), m_nextStagePointer
	));

	// アイテムボックス
	UiItembox* uiItembox = new UiItembox();
	m_rootNode->AddChild(new UiSwitchObject(
		uiItembox, uiInventory, uiInteractable, ItemboxPosition, Vector3(1, 1, 1), ItemboxInteractRadius)
	);

	// アイテムボックスの当たり判定
	m_rootNode->AddChild(new NoActionObject(
		ItemboxPosition + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Itembox)].OffsetY * ItemboxScale.y, 0),
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(ItemboxAngleY)), ItemboxScale,
		"Object", ModelData::List[static_cast<int>(ModelData::Name::Itembox)].ModelName, ItemboxInteractRadius
	));

	// キッチン
	UiKitchen* uiKitchen = new UiKitchen();
	m_rootNode->AddChild(new UiSwitchObject(
		uiKitchen, uiInventory, uiInteractable, KitchenPosition, KitchenScale, KitchenInteractRadius)
	);
	m_rootNode->AddChild(new NoActionObject(
		KitchenPosition + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Kitchen)].OffsetY * KitchenScale.y, 0),
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(KitchenAngleY)), KitchenScale,
		"Object", ModelData::List[static_cast<int>(ModelData::Name::Kitchen)].ModelName, KitchenCollisionRadius
	));

	// トレーダー
	UiTrader* uiTrader = nullptr;
	if (m_canSpawnTrader)
	{
		uiTrader = new UiTrader();
		m_rootNode->AddChild(new NoActionTrader());
		m_rootNode->AddChild(new UiSwitchObject(
			uiTrader, uiInventory, uiInteractable, NoActionTrader::Position, NoActionTrader::Scale, NoActionTrader::InteractRadius));
	}

	// 見た目のみのオブジェクト --------------------------------------------------------------------

	// ソファ
	m_rootNode->AddChild(new ModelDisplay(
		"sofa",
		ModelData::List[static_cast<int>(ModelData::Name::Sofa)].ModelName,
		SofaPosition + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Sofa)].OffsetY * SofaScale.y, 0),
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(SofaAngleY)), SofaScale
	));

	// ラジオ
	m_rootNode->AddChild(new ModelDisplay(
		"radio",
		ModelData::List[static_cast<int>(ModelData::Name::Radio)].ModelName,
		RadioPosition, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(RadioAngleY)), RadioScale
	));

	// コンクリ１
	m_rootNode->AddChild(new ModelDisplay(
		"concrete",
		ModelData::List[static_cast<int>(ModelData::Name::ConcreteBarrier)].ModelName,
		Concrete1Position, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(Concrete1AngleY)), Concrete1Scale
	));

	// コンクリ２
	m_rootNode->AddChild(new ModelDisplay(
		"concrete",
		ModelData::List[static_cast<int>(ModelData::Name::ConcreteBarrierIron)].ModelName,
		Concrete2Position, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(Concrete2AngleY)), Concrete2Scale
	));

	// ランプ
	m_rootNode->AddChild(new ModelDisplay(
		"lamp",
		ModelData::List[static_cast<int>(ModelData::Name::Lamp)].ModelName,
		LampPosition, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(LampAngleY)), LampScale
	));

	// UI ------------------------------------------------------------------------------------------

	// 太陽光
	m_rootNode->AddChild(new SunLight());

	// 体力
	m_rootNode->AddChild(new UiHealth());

	// 空腹度
	m_rootNode->AddChild(new UiSatietyLevel());

	// インベントリ
	m_rootNode->AddChild(uiInventory);

	// アイテムボックス
	m_rootNode->AddChild(uiItembox);

	// キッチン
	m_rootNode->AddChild(uiKitchen);

	if (m_canSpawnTrader && uiTrader != nullptr)
	{
		// トレーダー
		m_rootNode->AddChild(uiTrader);
	}

	// インタラクト可能UI
	m_rootNode->AddChild(uiInteractable);

	// 経過日数と時間帯
	m_rootNode->AddChild(new UiElapsedDayTime());

	// テキストメッセージUI
	m_rootNode->AddChild(uiTextMessage);

	// SE読み込み
	m_seTransition = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Transition)].SoundName, false);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seTransition, SoundData::List[static_cast<int>(SoundData::Name::Transition)].volume);
}

void SceneCamp::Finalize()
{
	// シーン遷移中のフェードアウト中に呼ばれる
	// ロードした画像の解放処理などを行う

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::Transition)].SoundName);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneCamp::Update()
{
	// シーン内の計算処理を行う

	// ノードの更新
	m_rootNode->TreeUpdate();

	// シーン遷移が可能になっていたら
	if (m_nextStagePointer != nullptr)
	{
		// 遷移先がキャンプシーンかどうか判定
		if (*m_nextStagePointer == static_cast<int>(StageData::Name::Camp))
		{
			// 空腹度を一つ減らす
			PlayerState::GetInstance()->SetSatietyLevel(-1);

			// 空腹度が0になったらゲームオーバー
			if (PlayerState::GetInstance()->GetSatietyLevel() <= 0)
			{
				return new SceneResult();
			}
			else
			{
				return new SceneCamp(0, m_playerController->GetPosition());
			}
		}
		else
		{
			// SE
			PlaySoundMem(m_seTransition, DX_PLAYTYPE_BACK);

			return new SceneField(*m_nextStagePointer, 0);
		}
	}

	return this;
}

void SceneCamp::Draw()
{
	// シーン内の描画処理を行う

	// ノードの描画
	m_rootNode->TreeDraw();

#ifndef NDEBUG
	// DEBUG:基点の表示
	DrawLine3D(Vector3(), Vector3(25, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(), Vector3(0, 25, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(), Vector3(0, 0, 25), GetColor(0, 0, 255));
#endif
}