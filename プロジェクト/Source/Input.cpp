#include "Input.h"
#include "Screen.h"
#include <memory.h>

void Input::Update()
{
	// 前フレームのキー入力状態を保存
	memcpy_s(m_keyStatePost, KeyStateNum, m_keyState, KeyStateNum);

	// 現在のキー入力状態を取得
	GetHitKeyStateAll(m_keyState);

	// マウス
	m_mouseButtonPost = m_mouseButton;
	m_mouseButton = DxLib::GetMouseInput();

	// カーソルがロックされているかどうか
	int MouseX = 0;
	int MouseY = 0;
	DxLib::GetMousePoint(&MouseX, &MouseY);
	m_prevMousePoint = m_cursorLock ? Screen::Center : m_mousePoint;
	m_mousePoint = Vector2(MouseX, MouseY);

	if (m_cursorLock)
	{
		SetMousePoint(static_cast<int>(Screen::Center.x), static_cast<int>(Screen::Center.y));
	}

	m_mouseWheel = GetMouseWheelRotVol();
	
	// マウスの左ボタンが押されたら、Zキーが入力されたことにする
	if (m_mouseButton & MOUSE_INPUT_LEFT) m_keyState[KEY_INPUT_Z] = 1;
}

// いずれかのキーが押された瞬間
bool Input::IsAnyKeyDown()
{
	// マウス
	if (m_mouseButton && m_mouseButton ^ m_mouseButtonPost)
	{
		return true;
	}

	// キーボード
	for (int key = 0; key < KeyStateNum; key++)
	{
		if (IsKeyDown(key))
		{
			return true;
		}
	}
	return false;
}

// カーソルロックの切り替えと後処理
void Input::CursorLock(bool state)
{
	// 変化しないならここで終了
	if (m_cursorLock == state) return;

	// カーソルの表示切替
	SetMouseDispFlag(!state);

	// カーソルをロックする場合の後処理
	if (state)
	{
		Vector2 offset = m_mousePoint - m_prevMousePoint;
		m_mousePoint = Screen::Center;
		m_prevMousePoint = m_mousePoint - offset;

		SetMousePoint(static_cast<int>(Screen::Center.x), static_cast<int>(Screen::Center.y));
	}

	// 状態を保持
	m_cursorLock = state;
}
