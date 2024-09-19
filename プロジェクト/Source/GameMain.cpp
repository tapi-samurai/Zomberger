#include "GameMain.h"
#include "Screen.h"
#include "Input.h"
#include "Time.h"
#include "Fade.h"
#include "SceneManager.h"
#include "ActorCollision3D.h"
#include <ctime>

#include "SceneTitle.h"
#include "SceneField.h"
#include "SceneCamp.h"

/*
ゲームメインクラス
DXLibやシーン管理クラス等の初期化から後始末、ゲームのメインループを行う

*/

GameMain::GameMain() :
	m_sceneManager(nullptr),
	m_screen(0)
{
	// DXLib 前処理
	SetMainWindowText("Zom Burger");				// ウィンドウタイトル
	SetAlwaysRunFlag(true);								// 非アクティブ時にも動作
	ChangeWindowMode(true);								// Window Mode
	SetGraphMode(Screen::Width, Screen::Height, 16);	// ウィンドウサイズとカラービット数を指定
	SetWaitVSyncFlag(false);							// ディスプレイの垂直同期を待たない

	std::srand(static_cast<unsigned int>(std::time(nullptr)));	// 乱数種を設定

	// DXLib 初期化
	if(DxLib_Init() == -1)
	{
		throw -1;
	}
}

GameMain::~GameMain()
{
	if(m_sceneManager)
	{
		delete m_sceneManager;
		m_sceneManager = nullptr;
	}

	// DXLib 終了
	DxLib_End();
}

void GameMain::Run()
{
	m_screen = MakeScreen(Screen::Width, Screen::Height);

	// シーンの初期化
	m_sceneManager = new SceneManager(new SceneTitle());
	
#ifndef NDEBUG
	// FPS計測
	double fps = 0.0;
	double frameTime = 0.0;
	int frameCount = 0;
#endif

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	// ライトの設定
	SetUseLighting(false);

	// メインループ
	while(ProcessMessage() == 0)
	{
		// フレームの開始時間を取得(単位 マイクロ秒)
		LONGLONG frameStartTime = GetNowHiPerformanceCount();

		// 入力システムの更新
		Input::GetInstance()->Update();

		// 時間の更新
		Time::GetInstance()->Update();

		// ESCキーが押されたらループから抜ける
		if(Input::GetInstance()->IsKeyDown(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// シーンの更新
		m_sceneManager->Update();

		// 当たり判定の処理
		ActorCollision3D::GetInstance()->Update();

		// 自作画面に描画
		SetDrawScreen(m_screen);

		// 自作画面をクリア
		ClearDrawScreen();

		// シーンの描画
		m_sceneManager->Draw();

#ifndef NDEBUG
		// DEBUG 当たり判定の範囲の描画
		ActorCollision3D::GetInstance()->DebugDraw();
		
		// FPS計測
		frameCount++;
		frameTime += Time::GetInstance()->GetDeltaTime();
		if(frameTime >= 1.0)
		{
			fps = frameCount / frameTime;
			frameCount = 0;
			frameTime = 0.0;
		}
//		DrawFormatString(Screen::Width - 110, 5, GetColor(255, 255, 255), "[Fps:%.2lf]", fps);
#endif

		// フェード
		Fade::GetInstance()->Update(m_screen);

		// 裏画面に描画
		SetDrawScreen(DX_SCREEN_BACK);

		// 裏画面をクリア
		ClearDrawScreen();

		// 自作画面を描画
		DrawGraph(0, 0, m_screen, false);

		// 画面更新
		ScreenFlip();

		// フレーム内の経過時間が60FPS(1/60秒)に達するまで待つ
		while (GetNowHiPerformanceCount() - frameStartTime < 1000000 / 60);
	}
}
