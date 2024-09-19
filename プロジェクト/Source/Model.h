#pragma once
#include "Transform3D.h"
#include "Animation3D.h"
#include <map>

/*
* Sprite�N���X��3D�o�[�W����
*/

class Model
{
private:
	const char* m_modelName;							// ���f���̃p�X
	int m_modelHandle;									// ���f���f�[�^
	std::map<const char*, Animation3D> m_animationList;	// ���A�j���[�V�������F�A�j���[�V�����f�[�^���y�A�̃��X�g
	const Animation3D* m_runningAnime;					// �Đ����̃A�j���[�V�������X�g�ւ̃|�C���^
	int m_attachIdx;									// �A�^�b�`����Ă���A�j���[�V�����̔ԍ�
	float m_elapsedCount;								// �o�ߎ���(�R�}��)
	int m_baseFrameIndex;								// �A�j���[�V�����ɂ��ړ��������Ȃ��ꍇ�̊�_�t���[���ԍ�

	void SetFrameUserLocalMatrix();	// ���f���̎w��t���[�����W�Ɋ�_���W�����킹��

public:
	// �R���X�g���N�^
	Model() :
		m_modelName(nullptr),
		m_modelHandle(0),
		m_runningAnime(nullptr),
		m_attachIdx(0),
		m_elapsedCount(0),
		m_baseFrameIndex(0) {}

	// ���f���̓ǂݍ���
	void Load();

	// ���f���̉��
	void Release();

	// �X�V
	void Update();

	// �`��
	void Draw(const Transform3D& transform);

	// ���f���o�^
	void ModelRegister(const char* modelName, int baseFrameIndex);

	// �A�j���[�V�����o�^
	void AnimeRegister(const char* animeName, const Animation3D& animation);

	// �A�j���[�V�����Đ�
	void Play(const char* animeName);

	// �A�j���[�V�����̌��ݎ��Ԃ��擾
	float GetAnimeTime() { return m_elapsedCount; }

	// �A�j���[�V�����̑��Đ����Ԃ��擾
	float GetAnimeTotalTime() { return m_runningAnime ? MV1GetAttachAnimTotalTime(m_modelHandle, m_attachIdx) : 0.0f; }

	// ���f���n���h�����擾
	int GetModelHandle() { return m_modelHandle; }

	// �A�j���[�V�������I���������ǂ���
	bool IsFinishAnime() { return m_runningAnime ? m_elapsedCount >= GetAnimeTotalTime() : true; }
 };
