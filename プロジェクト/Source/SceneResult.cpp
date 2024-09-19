#include "SceneResult.h"
#include "Node.h"
#include "SceneTitle.h"
#include "Input.h"
#include "TimeProgressManager.h"
#include "FontLoader.h"
#include "BgmPlayer.h"
#include "SoundData.h"
#include "FontData.h"

void SceneResult::Initialize()
{
	// シーン遷移することが決まったら即呼ばれる
	// 画像のロードなどを行う

	m_rootNode = new Node();

	// フォントデータの読み込み
	m_fontHandle = LoadFontDataToHandle(FontData::List[static_cast<int>(FontData::Name::CinecaptionAll)]);

	// BGMを停止
	BgmPlayer::GetInstance()->Stop();
}

void SceneResult::Finalize()
{
	// シーン遷移中のフェードアウト中に呼ばれる
	// ロードした画像の解放処理などを行う

	// フォントの削除
	DeleteFontToHandle(m_fontHandle);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneResult::Update()
{
	// シーン内の計算処理を行う

	// ノードの更新
	m_rootNode->TreeUpdate();

	// 何らかのキーが押されたら
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		return new SceneTitle();
	}

	return this;
}

void SceneResult::Draw()
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

	// メッセージを表示
	DrawFormatStringToHandle(
		TextHeight, TextWidth, GetColor(255, 255, 255), m_fontHandle,
		"\"%2d日\"生き残り、力尽きた...", TimeProgressManager::GetInstance()->GetElapsedDay() + 1
	);
}