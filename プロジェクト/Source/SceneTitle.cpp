#include "SceneTitle.h"
#include "Node.h"
#include "DxLib.h"
#include "Input.h"
#include "Actor3D.h"
#include "ModelData.h"
#include "Camera.h"
#include "Math.h"
#include "Actor.h"
#include "Screen.h"
#include "Time.h"
#include "SceneCamp.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "BgmPlayer.h"
#include "ItemField.h"
#include "TimeProgressManager.h"
#include "PlayerState.h"
#include "FontData.h"

void SceneTitle::Initialize()
{
	// シーン遷移することが決まったら即呼ばれる
	// 画像のロードなどを行う

	m_rootNode = new Node();

	// フォント読み込み
	m_fontHandle = LoadFontDataToHandle(FontData::List[static_cast<int>(FontData::Name::CinecaptionAll)]);

	// BGM読み込み
	BgmPlayer::GetInstance()->Register(SoundData::List[static_cast<int>(SoundData::Name::Bgm)].SoundName);

	// BGM設定
	SoundEffect::GetInstance()->SetSoundEffect2D(
		BgmPlayer::GetInstance()->GetBgmHandle(), SoundData::List[static_cast<int>(SoundData::Name::Bgm)].volume
	);

	// アイテムをリセット
	ItemField::GetInstance()->Initialize();

	// 時間の進行をリセット
	TimeProgressManager::GetInstance()->DayInitialization();
	TimeProgressManager::GetInstance()->TimezoneInitialization();

	// 体力と空腹度をリセット
	PlayerState::GetInstance()->SetHealth(PlayerState::MaxHealth);
	PlayerState::GetInstance()->SetSatietyLevel(PlayerState::MaxSatietyLevel);
}

void SceneTitle::Finalize()
{
	// シーン遷移中のフェードアウト中に呼ばれる
	// ロードした画像の解放処理などを行う

	// フォントの削除
	DeleteFontToHandle(m_fontHandle);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneTitle::Update()
{
	// シーン内の計算処理を行う

	// ノードの更新
	m_rootNode->TreeUpdate();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// PressAnyKeyの表示フラグを一定間隔で切り替える
	if (m_elapsedTime > PressAnyKeyBlinkingInterval)
	{
		m_elapsedTime = 0;
		m_pressAnyKeyFlag = !m_pressAnyKeyFlag;
	}

	if (Input::GetInstance()->IsAnyKeyDown())
	{
		// BGMを再生
		BgmPlayer::GetInstance()->Play();

		return new SceneCamp(0);
	}

	return this;
}

void SceneTitle::Draw()
{
	// シーン内の描画処理を行う
	
	// ノードの描画
	m_rootNode->TreeDraw();

	// Press Any Key表示
	if (m_pressAnyKeyFlag)
	{
		// 文字列の幅を取得
		int width = GetDrawFormatStringWidthToHandle(m_fontHandle, "PRESS ANY KEY");

		// 表示
		DrawFormatStringToHandle(
			static_cast<int>(Screen::Width / 2.0f - width / 2.0f),
			static_cast<int>(PressAnyKeyHeight),
			GetColor(255, 255, 255), m_fontHandle, "PRESS ANY KEY"
		);
	}
}