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
�Q�[�����C���N���X
DXLib��V�[���Ǘ��N���X���̏����������n���A�Q�[���̃��C�����[�v���s��

*/

GameMain::GameMain() :
	m_sceneManager(nullptr),
	m_screen(0)
{
	// DXLib �O����
	SetMainWindowText("Zom Burger");				// �E�B���h�E�^�C�g��
	SetAlwaysRunFlag(true);								// ��A�N�e�B�u���ɂ�����
	ChangeWindowMode(true);								// Window Mode
	SetGraphMode(Screen::Width, Screen::Height, 16);	// �E�B���h�E�T�C�Y�ƃJ���[�r�b�g�����w��
	SetWaitVSyncFlag(false);							// �f�B�X�v���C�̐���������҂��Ȃ�

	std::srand(static_cast<unsigned int>(std::time(nullptr)));	// �������ݒ�

	// DXLib ������
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

	// DXLib �I��
	DxLib_End();
}

void GameMain::Run()
{
	m_screen = MakeScreen(Screen::Width, Screen::Height);

	// �V�[���̏�����
	m_sceneManager = new SceneManager(new SceneTitle());
	
#ifndef NDEBUG
	// FPS�v��
	double fps = 0.0;
	double frameTime = 0.0;
	int frameCount = 0;
#endif

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �y�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(TRUE);

	// ���C�g�̐ݒ�
	SetUseLighting(false);

	// ���C�����[�v
	while(ProcessMessage() == 0)
	{
		// �t���[���̊J�n���Ԃ��擾(�P�� �}�C�N���b)
		LONGLONG frameStartTime = GetNowHiPerformanceCount();

		// ���̓V�X�e���̍X�V
		Input::GetInstance()->Update();

		// ���Ԃ̍X�V
		Time::GetInstance()->Update();

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if(Input::GetInstance()->IsKeyDown(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// �V�[���̍X�V
		m_sceneManager->Update();

		// �����蔻��̏���
		ActorCollision3D::GetInstance()->Update();

		// �����ʂɕ`��
		SetDrawScreen(m_screen);

		// �����ʂ��N���A
		ClearDrawScreen();

		// �V�[���̕`��
		m_sceneManager->Draw();

#ifndef NDEBUG
		// DEBUG �����蔻��͈̔͂̕`��
		ActorCollision3D::GetInstance()->DebugDraw();
		
		// FPS�v��
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

		// �t�F�[�h
		Fade::GetInstance()->Update(m_screen);

		// ����ʂɕ`��
		SetDrawScreen(DX_SCREEN_BACK);

		// ����ʂ��N���A
		ClearDrawScreen();

		// �����ʂ�`��
		DrawGraph(0, 0, m_screen, false);

		// ��ʍX�V
		ScreenFlip();

		// �t���[�����̌o�ߎ��Ԃ�60FPS(1/60�b)�ɒB����܂ő҂�
		while (GetNowHiPerformanceCount() - frameStartTime < 1000000 / 60);
	}
}
