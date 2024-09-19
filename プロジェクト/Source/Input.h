#pragma once
#include "Vector2.h"
#include "DxLib.h"

// RETURN�L�[�ɕʖ�������
#define KEY_INPUT_ENTER KEY_INPUT_RETURN

class Input
{
private:
	// �L�[�{�[�h
	static constexpr int KeyStateNum = 256;	// �L�[���͏�Ԃ̗v�f��

	char m_keyState[KeyStateNum];		// ���t���[���̃L�[�̓��͏��
	char m_keyStatePost[KeyStateNum];	// �O�t���[���̃L�[�̓��͏��

	// �}�E�X
	Vector2 m_mousePoint;		// ���t���[���̃}�E�X���W
	Vector2 m_prevMousePoint;	// �O�t���[���̃}�E�X���W
	int m_mouseWheel;
	int m_mouseButton;
	int m_mouseButtonPost;
	bool m_cursorLock;

	// �R���X�g���N�^
	Input() :
		m_keyState{0},
		m_keyStatePost{0},
		m_mouseWheel(0),
		m_mouseButton(0),
		m_mouseButtonPost(0),
		m_cursorLock(false) {}

public:
	// �V���O���g��
	static Input* GetInstance()
	{
		static Input instance;
		return &instance;
	}

	// ���t���[���Ă�ł��炤�X�V����
	void Update();

	// �L�[�������ꂽ�u��
	bool IsKeyDown(int key) { return !m_keyStatePost[key] && m_keyState[key]; }

	// �L�[��������Ă���
	bool IsKeyPress(int key) { return m_keyState[key]; }

	// �L�[�������ꂽ�u��
	bool IsKeyUp(int key) { return m_keyStatePost[key] && !m_keyState[key]; }

	// �����ꂩ�̃L�[�������ꂽ�u��
	bool IsAnyKeyDown();

	// �J�[�\���̃��b�N�؂�ւ�
	void CursorLock(bool state);


	// �}�E�X���W���擾
	Vector2 GetMousePoint() { return m_mousePoint; }

	// �}�E�X�̑O�t���[������̈ړ��������擾
	Vector2 GetMoveMousePoint() { return m_mousePoint - m_prevMousePoint; }

	// �}�E�X�z�C�[�����擾
	int GetMouseWheelRot() { return m_mouseWheel; };

	// �}�E�X�{�^���������ꂽ�u��
	bool IsMouseDown(int button) { return (m_mouseButton & button) && !(m_mouseButtonPost & button); }

	// �}�E�X�{�^����������Ă���
	bool IsMousePress(int button) { return (m_mouseButton & button); }

	// �}�E�X�{�^���������ꂽ�u��
	bool IsMouseUp(int button) { return !(m_mouseButton & button) && (m_mouseButtonPost & button); }
};
