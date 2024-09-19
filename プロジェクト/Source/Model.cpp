#include "Model.h"
#include "Time.h"
#include "ModelLoader.h"

// ���f���ƃA�j���[�V�����̓ǂݍ���
void Model::Load()
{
	// ���f���f�[�^���Ȃ��Ȃ牽�����[�h���Ȃ�
	if (!m_modelName) return;

	// ���f��
	m_modelHandle = ModelLoader::GetInstance()->Load(m_modelName);

	// �A�j���[�V����
	for (auto& pair : m_animationList)
	{
		// ���X�g���灃�A�j���[�V�������F�A�j���[�V�����f�[�^���̃y�A�����o��
		// pair.first �F�A�j���[�V������
		// pair.second�F�A�j���[�V�����f�[�^

		// �A�j���[�V�����f�[�^����A�j���[�V�����p�X�����o���ă��[�h
		pair.second.animationId = ModelLoader::GetInstance()->Load(pair.second.animationName);

		// �ŏ��ɋ쓮����A�j���͏��߂ēo�^��������
		if (!m_runningAnime)
		{
			m_runningAnime = &m_animationList.begin()->second;
			m_attachIdx = MV1AttachAnim(m_modelHandle, 0, m_runningAnime->animationId);
		}
	}

	// �A�j���[�V�����ɂ�郂�f���̈ړ���ݒ�
	SetFrameUserLocalMatrix();
}

// ���f���ƃA�j���[�V�����̉��
void Model::Release()
{
	if (m_modelHandle)
	{
		// �I���W�i���f�[�^
		ModelLoader::GetInstance()->Delete(m_modelName, m_modelHandle);
	}

	for (auto& pair : m_animationList)
	{
		// �I���W�i���f�[�^
		ModelLoader::GetInstance()->Delete(pair.second.animationName, pair.second.animationId);
	}
}

// �X�V
void Model::Update()
{
	// �A�j���[�V�������I�����Ă��Ȃ���
	if (IsFinishAnime()) return;

	// �o�ߎ��Ԃ����A�j���[�V�������Ԃ𒴂����烋�[�v����
	m_elapsedCount += Time::GetInstance()->GetLocalDeltaTime() * m_runningAnime->playSpeed * m_runningAnime->CountSec;

	if (IsFinishAnime())
	{
		if (m_runningAnime->loopFlag)
		{
			// ���[�v�L��Ȃ�ŏ��̃R�}�ɖ߂�
			m_elapsedCount = 0;
		}
	}
}

// �`��
void Model::Draw(const Transform3D& transform)
{
	// ���f�����Ȃ���Ή������Ȃ�
	if (!m_modelHandle) return;

	// ���W�Ɖ�]�Ɗg�k���擾
	Vector3 position = transform.position;
	Quaternion rotation = transform.rotation;
	Vector3 scale = transform.scale;

	// �A�j���[�V����������ΐݒ�
	if (m_runningAnime)
	{
		// �A�j���[�V������ݒ�
		MV1SetAttachAnimTime(m_modelHandle, m_attachIdx, m_elapsedCount);
	}

	// ���f���̏����s��Őݒ�
	MATRIX matrix = MGetIdent();
	matrix = MMult(matrix, MGetScale(Vector3(scale.x, scale.y, scale.z)));	// �g�k
	matrix = MMult(matrix, rotation);								// ��]
	matrix = MMult(matrix, MGetTranslate(position));				// ���s�ړ��@�̏�������
	MV1SetMatrix(m_modelHandle, matrix);

	// �\��
	MV1DrawModel(m_modelHandle);
}

// ���f���o�^
void Model::ModelRegister(const char* modelName, int baseFrameIndex)
{
	m_modelName = modelName;
	m_baseFrameIndex = baseFrameIndex;
}

// �A�j���[�V�����o�^
void Model::AnimeRegister(const char* animeName, const Animation3D& animation)
{
	// �A�j���[�V�����̓o�^
	m_animationList.emplace(animeName, animation);
}

// �A�j���[�V�����Đ�
void Model::Play(const char* animeName)
{
	// �o�^���ꂽ�A�j���[�V���������X�g����擾
	const auto& pair = m_animationList.find(animeName);

	// �o�^����Ă��Ȃ��A�j��
	if (pair == m_animationList.end())
	{
		return;
	}

	// ���݂Ɠ����A�j��
	if (m_runningAnime == &pair->second)
	{
		return;
	}

	// �A�j���[�V�����؂�ւ�
	m_runningAnime = &pair->second;
	MV1DetachAnim(m_modelHandle, m_attachIdx);
	m_attachIdx = MV1AttachAnim(m_modelHandle, 0, m_runningAnime->animationId);
	m_elapsedCount = 0;

	// �A�j���[�V�����ɂ�郂�f���̈ړ���ݒ�
	SetFrameUserLocalMatrix();
}

void Model::SetFrameUserLocalMatrix()
{
	// �A�j���[�V������������΃X�L�b�v
	if (m_runningAnime == nullptr) return;

	// ���f���̎w��t���[�����W�Ɋ�_���W�����킹��
	if (m_runningAnime->animationMove == false)
	{
		MV1SetFrameUserLocalMatrix(m_modelHandle, m_baseFrameIndex, MGetIdent());
	}
	else
	{
		MV1ResetFrameUserLocalMatrix(m_modelHandle, m_baseFrameIndex);
	}
}