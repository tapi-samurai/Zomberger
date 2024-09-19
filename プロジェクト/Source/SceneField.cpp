#include "SceneField.h"
#include "Input.h"
#include "Quad3D.h"
#include "SceneTitle.h"
#include "Time.h"
#include "Screen.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "FontLoader.h"
#include "SoundData.h"
#include "Camera.h"
#include "Math.h"
#include "PlayerController.h"
#include "SceneTransitionQuad.h"
#include "PlayerInteractLandmark.h"
#include "ItemField.h"
#include "ModelData.h"
#include "StageData.h"
#include "SceneCamp.h"
#include "UiInventory.h"
#include "ItemData.h"
#include "TimeProgressManager.h"
#include "UiElapsedDayTime.h"
#include "EnemySpawner.h"
#include "FieldObjectGenerator.h"
#include "PlayerScoopAttack.h"
#include "PlayerState.h"
#include "SceneResult.h"
#include "SunLight.h"
#include "UiInteractItemGage.h"
#include "UiHealth.h"
#include "UiSatietyLevel.h"
#include "UiInteractable.h"
#include "CameraData.h"
#include "UiTextMessage.h"

// ゲームシーンクラス

void SceneField::Initialize()
{
	// シーン遷移することが決まったら即呼ばれる
	// 画像のロードなどを行う

	m_rootNode = new Node();

	// 時間帯を進める
	TimeProgressManager::GetInstance()->AddElapsedTimezone(m_addTimezoneValue);

	// 経過秒をリセット
	TimeProgressManager::GetInstance()->SecondInitialization();

	// インベントリ生成
	UiInventory* uiInventory = new UiInventory();

	// インタラクト可能UI
	UiInteractable* const uiInteractable = new UiInteractable();

	// テキストメッセージUI
	UiTextMessage* const uiTextMessage = new UiTextMessage();

	// カメラ
	int cameraNum = rand() % CameraData::Field.size();
	m_rootNode->AddChild(new Camera(
		CameraData::Field[cameraNum].CameraPosition,
		Quaternion::CreateRotationAxis(Vector3::Right(), Math::DegToRad(CameraData::Field[cameraNum].CameraAngleX)) *
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(CameraData::Field[cameraNum].CameraAngleY)),
		1.0f, 200.0f
	));

	// 床
	m_rootNode->AddChild(new Quad3D(
		Vector3(), Quaternion::CreateRotationAxis(Vector3::Right(), Math::DegToRad(-90.0f)), FloorHeight, FloorWidth,
		"Floor", StageData::List[m_currentStageNum].FloorTextureName)
	);

	// 見えない壁とシーン遷移用の当たり判定を生成
	for (int i = 0; i < 4; i++)
	{
		// 表示用の変数を用意
		float length = (i + 1) % 2 != 0 ? FloorHeight / 2 : FloorWidth / 2;
		float wallWidth = (i + 1) % 2 == 0 ? FloorHeight : FloorWidth;
		bool isDisplay = i == 0 || i == 1 ? true : false;
		Quaternion rotation = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(-90.0f + 90.0f * i));

		// 左から時計回りに壁を生成
		m_rootNode->AddChild(new Quad3D(
			Vector3::Forward() * length * rotation + Vector3(0, WallHeight / 2, 0),
			rotation, WallHeight, wallWidth,
			"Wall", StageData::List[m_currentStageNum].WallTextureName, isDisplay, GetColorU8(100, 100, 100, 200)
		));

		// シーン遷移先のステージ番号をランダムで設定
		// 下側は必ずキャンプに繋がるように
		int nextStageNum = i == 3 ? static_cast<int>(StageData::Name::Camp) : rand() % static_cast<int>(StageData::Name::Length);

		// シーン遷移用の当たり判定を持つオブジェクトを生成
		m_rootNode->AddChild(new SceneTransitionQuad(
			uiInteractable, uiTextMessage, m_nextStagePointer, nextStageNum,
			Vector3::Forward() * (length - SceneTransitionQuadToWallLength) * rotation + Vector3(0, WallHeight / 2, 0),
			rotation, FloorWidth, WallHeight
			));
	}

	// プレイヤー
	Vector3 playerPos = PlayerInitialPosition;
	Quaternion playerRot = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(PlayerInitialAngleY));
	PlayerController* playerController = new PlayerController(playerPos, playerRot, uiInventory);
	m_rootNode->AddChild(playerController);

	// スコップ攻撃オブジェクト
	PlayerScoopAttack* playerScoopAttack = new PlayerScoopAttack(playerController);
	playerController->SetPlayerScoopAttack(playerScoopAttack);
	playerController->AddChild(playerScoopAttack);

	// シーン遷移用の当たり判定を検知するオブジェクト
	m_playerInteractLandmark = new PlayerInteractLandmark(playerController);
	playerController->AddChild(m_playerInteractLandmark);

	// 敵スポナー
	m_enemySpawner = new EnemySpawner(playerController);
	m_rootNode->AddChild(m_enemySpawner);

	// アイテム用ゲージUI
	UiInteractItemGage* const uiInteractItemGage = new UiInteractItemGage();

	// ステージオブジェクト生成器
	m_rootNode->AddChild(new FieldObjectGenerator(playerController, m_enemySpawner, uiInteractItemGage, m_currentStageNum));

	// UI --------------------------------------------------------------------------------

	// 太陽光
	m_rootNode->AddChild(new SunLight());

	// インベントリ
	m_rootNode->AddChild(uiInventory);

	// インタラクト可能UI
	m_rootNode->AddChild(uiInteractable);

	// 経過日数と時間帯
	m_rootNode->AddChild(new UiElapsedDayTime());

	// 体力
	m_rootNode->AddChild(new UiHealth());

	// 空腹度
	m_rootNode->AddChild(new UiSatietyLevel());

	// アイテム用ゲージ
	m_rootNode->AddChild(uiInteractItemGage);

	// テキストメッセージUI
	m_rootNode->AddChild(uiTextMessage);

	// SE読み込み
	m_seTransition = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Transition)].SoundName, false);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seTransition, SoundData::List[static_cast<int>(SoundData::Name::Transition)].volume);
}

void SceneField::Finalize()
{
	// シーン遷移中のフェードアウト中に呼ばれる
	// ロードした画像の解放処理などを行う

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::Transition)].SoundName);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneField::Update()
{
	// シーン内の計算処理を行う

	// ノードの更新
	m_rootNode->TreeUpdate();

	// フィールドに入ってから経過した秒数を進める
	TimeProgressManager::GetInstance()->AddElapsedSecond(Time::GetInstance()->GetLocalDeltaTime());

	// シーン遷移が可能になっていたら
	if (m_nextStagePointer != nullptr)
	{
		// SE
		PlaySoundMem(m_seTransition, DX_PLAYTYPE_BACK);

		// 遷移先がキャンプシーンかどうか判定
		if (*m_nextStagePointer == static_cast<int>(StageData::Name::Camp))
		{
			return new SceneCamp();
		}
		else
		{
			return new SceneField(*m_nextStagePointer);
		}
	}

	if (PlayerState::GetInstance()->GetHealth() <= 0)
	{
		m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

		if (m_elapsedTime >= SceneTransitionDelay)
		{
			// リザルトシーンに遷移
			return new SceneResult();
		}
	}

	return this;
}

void SceneField::Draw()
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
