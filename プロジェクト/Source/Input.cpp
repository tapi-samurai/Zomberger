#include "Input.h"
#include "Screen.h"
#include <memory.h>

void Input::Update()
{
	// �O�t���[���̃L�[���͏�Ԃ�ۑ�
	memcpy_s(m_keyStatePost, KeyStateNum, m_keyState, KeyStateNum);

	// ���݂̃L�[���͏�Ԃ��擾
	GetHitKeyStateAll(m_keyState);

	// �}�E�X
	m_mouseButtonPost = m_mouseButton;
	m_mouseButton = DxLib::GetMouseInput();

	// �J�[�\�������b�N����Ă��邩�ǂ���
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
	
	// �}�E�X�̍��{�^���������ꂽ��AZ�L�[�����͂��ꂽ���Ƃɂ���
	if (m_mouseButton & MOUSE_INPUT_LEFT) m_keyState[KEY_INPUT_Z] = 1;
}

// �����ꂩ�̃L�[�������ꂽ�u��
bool Input::IsAnyKeyDown()
{
	// �}�E�X
	if (m_mouseButton && m_mouseButton ^ m_mouseButtonPost)
	{
		return true;
	}

	// �L�[�{�[�h
	for (int key = 0; key < KeyStateNum; key++)
	{
		if (IsKeyDown(key))
		{
			return true;
		}
	}
	return false;
}

// �J�[�\�����b�N�̐؂�ւ��ƌ㏈��
void Input::CursorLock(bool state)
{
	// �ω����Ȃ��Ȃ炱���ŏI��
	if (m_cursorLock == state) return;

	// �J�[�\���̕\���ؑ�
	SetMouseDispFlag(!state);

	// �J�[�\�������b�N����ꍇ�̌㏈��
	if (state)
	{
		Vector2 offset = m_mousePoint - m_prevMousePoint;
		m_mousePoint = Screen::Center;
		m_prevMousePoint = m_mousePoint - offset;

		SetMousePoint(static_cast<int>(Screen::Center.x), static_cast<int>(Screen::Center.y));
	}

	// ��Ԃ�ێ�
	m_cursorLock = state;
}
