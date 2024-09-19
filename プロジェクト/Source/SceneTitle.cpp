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
	// �V�[���J�ڂ��邱�Ƃ����܂����瑦�Ă΂��
	// �摜�̃��[�h�Ȃǂ��s��

	m_rootNode = new Node();

	// �t�H���g�ǂݍ���
	m_fontHandle = LoadFontDataToHandle(FontData::List[static_cast<int>(FontData::Name::CinecaptionAll)]);

	// BGM�ǂݍ���
	BgmPlayer::GetInstance()->Register(SoundData::List[static_cast<int>(SoundData::Name::Bgm)].SoundName);

	// BGM�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(
		BgmPlayer::GetInstance()->GetBgmHandle(), SoundData::List[static_cast<int>(SoundData::Name::Bgm)].volume
	);

	// �A�C�e�������Z�b�g
	ItemField::GetInstance()->Initialize();

	// ���Ԃ̐i�s�����Z�b�g
	TimeProgressManager::GetInstance()->DayInitialization();
	TimeProgressManager::GetInstance()->TimezoneInitialization();

	// �̗͂Ƌ󕠓x�����Z�b�g
	PlayerState::GetInstance()->SetHealth(PlayerState::MaxHealth);
	PlayerState::GetInstance()->SetSatietyLevel(PlayerState::MaxSatietyLevel);
}

void SceneTitle::Finalize()
{
	// �V�[���J�ڒ��̃t�F�[�h�A�E�g���ɌĂ΂��
	// ���[�h�����摜�̉�������Ȃǂ��s��

	// �t�H���g�̍폜
	DeleteFontToHandle(m_fontHandle);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneTitle::Update()
{
	// �V�[�����̌v�Z�������s��

	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// PressAnyKey�̕\���t���O�����Ԋu�Ő؂�ւ���
	if (m_elapsedTime > PressAnyKeyBlinkingInterval)
	{
		m_elapsedTime = 0;
		m_pressAnyKeyFlag = !m_pressAnyKeyFlag;
	}

	if (Input::GetInstance()->IsAnyKeyDown())
	{
		// BGM���Đ�
		BgmPlayer::GetInstance()->Play();

		return new SceneCamp(0);
	}

	return this;
}

void SceneTitle::Draw()
{
	// �V�[�����̕`�揈�����s��
	
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();

	// Press Any Key�\��
	if (m_pressAnyKeyFlag)
	{
		// ������̕����擾
		int width = GetDrawFormatStringWidthToHandle(m_fontHandle, "PRESS ANY KEY");

		// �\��
		DrawFormatStringToHandle(
			static_cast<int>(Screen::Width / 2.0f - width / 2.0f),
			static_cast<int>(PressAnyKeyHeight),
			GetColor(255, 255, 255), m_fontHandle, "PRESS ANY KEY"
		);
	}
}