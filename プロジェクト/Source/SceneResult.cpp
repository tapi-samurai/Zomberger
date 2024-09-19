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
	// �V�[���J�ڂ��邱�Ƃ����܂����瑦�Ă΂��
	// �摜�̃��[�h�Ȃǂ��s��

	m_rootNode = new Node();

	// �t�H���g�f�[�^�̓ǂݍ���
	m_fontHandle = LoadFontDataToHandle(FontData::List[static_cast<int>(FontData::Name::CinecaptionAll)]);

	// BGM���~
	BgmPlayer::GetInstance()->Stop();
}

void SceneResult::Finalize()
{
	// �V�[���J�ڒ��̃t�F�[�h�A�E�g���ɌĂ΂��
	// ���[�h�����摜�̉�������Ȃǂ��s��

	// �t�H���g�̍폜
	DeleteFontToHandle(m_fontHandle);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneResult::Update()
{
	// �V�[�����̌v�Z�������s��

	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();

	// ���炩�̃L�[�������ꂽ��
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		return new SceneTitle();
	}

	return this;
}

void SceneResult::Draw()
{
	// �V�[�����̕`�揈�����s��

	// �m�[�h�̕`��
	m_rootNode->TreeDraw();

#ifndef NDEBUG
	// DEBUG:��_�̕\��
	DrawLine3D(Vector3(), Vector3(25, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(), Vector3(0, 25, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(), Vector3(0, 0, 25), GetColor(0, 0, 255));
#endif

	// ���b�Z�[�W��\��
	DrawFormatStringToHandle(
		TextHeight, TextWidth, GetColor(255, 255, 255), m_fontHandle,
		"\"%2d��\"�����c��A�͐s����...", TimeProgressManager::GetInstance()->GetElapsedDay() + 1
	);
}