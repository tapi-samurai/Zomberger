#include "SceneManager.h"
#include "SceneBase.h"
#include "Fade.h"
#include "DxLib.h"

/*
シーン管理クラス
シーンの切り替え処理を担う

*/

SceneManager::~SceneManager()
{
	// ロード中に表示する画像の解放
	m_loadingScreen.Release();

	// 現在のシーンと次のシーンが同じなら、重複deleteしないようにしておく
	if(m_scene == m_nextScene)
	{
		m_nextScene = nullptr;
	}

	// シーンの破棄
	if(m_scene)
	{
		m_scene->Finalize();
		delete m_scene;
		m_scene = nullptr;
	}
	if(m_nextScene)
	{
		m_nextScene->Finalize();
		delete m_nextScene;
		m_nextScene = nullptr;
	}
}

void SceneManager::Update()
{
	// シーン遷移のフェーズによって処理を切り替える
	switch(m_phase)
	{
	// 実行中
	case Phase::Run:
		// 現在のシーンの更新処理
		m_nextScene = m_scene->Update();
		if(m_nextScene != m_scene)
		{
			// フェードアウト開始
			m_phase = Phase::FadeOut;
			Fade::GetInstance()->StartFadeOut();
		}
		break;

	// フェードアウト
	case Phase::FadeOut:
		// フェードアウトが終わるまで待機
		if (!Fade::GetInstance()->IsFade())
		{
			m_phase = Phase::SwitchScene;
		}
		break;

	// シーン切り替え
	case Phase::SwitchScene:
		// 非同期読み込み設定を有効化
		SetUseASyncLoadFlag(TRUE);

		// 遷移先シーンの初期化
		m_nextScene->Initialize();

		// 非同期読み込み設定を無効化
		SetUseASyncLoadFlag(FALSE);

		// 現在実行中のシーンの終了処理と解放を行う
		if(m_scene)
		{
			m_scene->Finalize();
			delete m_scene;
			m_scene = nullptr;
		}

		// 遷移先シーンを現在実行中のシーンにする
		m_scene = m_nextScene;

		// m_nextSceneの所有権をm_sceneに渡したので、m_nextSceneはdeleteしない
		// m_nextSceneのポインタを消すに留める
		m_nextScene = nullptr;

		// フェーズ進行
		m_phase = Phase::AwaitAsyncLoad;
		break;

	// 非同期読み込みが終わるまで待機
	case Phase::AwaitAsyncLoad:
		if (GetASyncLoadNum() == 0)
		{
			// フェーズ進行
			m_phase = Phase::Run;

			// フェードイン
			Fade::GetInstance()->StartFadeIn(0.2f);
		}
		break;
	}
}

void SceneManager::Draw()
{
	// フェーズ毎に描画するものを切り替える
	switch(m_phase)
	{
	case Phase::Run:
	case Phase::FadeOut:
		if(m_scene)
		{
			m_scene->Draw();
		}
		break;

	case Phase::AwaitAsyncLoad:
	case Phase::SwitchScene:
		// 読み込み中用の画像を表示
		m_loadingScreen.Draw();
	}
}
