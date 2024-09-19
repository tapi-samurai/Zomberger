#pragma once
#include "Vector2.h"
#include "DxLib.h"

// RETURNキーに別名をつける
#define KEY_INPUT_ENTER KEY_INPUT_RETURN

class Input
{
private:
	// キーボード
	static constexpr int KeyStateNum = 256;	// キー入力状態の要素数

	char m_keyState[KeyStateNum];		// 現フレームのキーの入力状態
	char m_keyStatePost[KeyStateNum];	// 前フレームのキーの入力状態

	// マウス
	Vector2 m_mousePoint;		// 現フレームのマウス座標
	Vector2 m_prevMousePoint;	// 前フレームのマウス座標
	int m_mouseWheel;
	int m_mouseButton;
	int m_mouseButtonPost;
	bool m_cursorLock;

	// コンストラクタ
	Input() :
		m_keyState{0},
		m_keyStatePost{0},
		m_mouseWheel(0),
		m_mouseButton(0),
		m_mouseButtonPost(0),
		m_cursorLock(false) {}

public:
	// シングルトン
	static Input* GetInstance()
	{
		static Input instance;
		return &instance;
	}

	// 毎フレーム呼んでもらう更新処理
	void Update();

	// キーが押された瞬間
	bool IsKeyDown(int key) { return !m_keyStatePost[key] && m_keyState[key]; }

	// キーが押されている
	bool IsKeyPress(int key) { return m_keyState[key]; }

	// キーが離された瞬間
	bool IsKeyUp(int key) { return m_keyStatePost[key] && !m_keyState[key]; }

	// いずれかのキーが押された瞬間
	bool IsAnyKeyDown();

	// カーソルのロック切り替え
	void CursorLock(bool state);


	// マウス座標を取得
	Vector2 GetMousePoint() { return m_mousePoint; }

	// マウスの前フレームからの移動距離を取得
	Vector2 GetMoveMousePoint() { return m_mousePoint - m_prevMousePoint; }

	// マウスホイールを取得
	int GetMouseWheelRot() { return m_mouseWheel; };

	// マウスボタンが押された瞬間
	bool IsMouseDown(int button) { return (m_mouseButton & button) && !(m_mouseButtonPost & button); }

	// マウスボタンが押されている
	bool IsMousePress(int button) { return (m_mouseButton & button); }

	// マウスボタンが離された瞬間
	bool IsMouseUp(int button) { return !(m_mouseButton & button) && (m_mouseButtonPost & button); }
};
