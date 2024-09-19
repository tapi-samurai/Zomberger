#pragma once
#include "LoadingScreen.h"

class SceneBase;

class SceneManager
{
private:
	// シーン遷移のフェーズを列挙
	enum class Phase
	{
		Run,			// 実行中
		FadeOut,		// フェードアウト
		SwitchScene,	// シーン切り替え
		AwaitAsyncLoad,	// 非同期読み込み中
	};

	SceneBase* m_scene;				// 現在のシーン
	SceneBase* m_nextScene;			// 遷移先のシーン
	Phase m_phase;					// シーン遷移のフェーズ
	LoadingScreen m_loadingScreen;	// ロード中に表示する画像

public:
	SceneManager(SceneBase* initScene) :
		m_scene(nullptr),
		m_nextScene(initScene),
		m_phase(Phase::SwitchScene) {}

	~SceneManager();

	void Update();
	void Draw();
};
