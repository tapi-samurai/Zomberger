#include "Actor3D.h"
#include "ActorCollision3D.h"
#include "BitFlag.h"

#include <typeinfo>

// �R���X�g���N�^
Actor3D::Actor3D(const char* name, const char* modelName, int baseFrameIndex, bool isActive) :
	m_name(name),
	m_model(nullptr),
	m_collider(nullptr),
	m_bitFlag(nullptr),
	m_isActive(isActive)
{;
	if (modelName)
	{
		m_model = new Model();
		m_model->ModelRegister(modelName, baseFrameIndex);
	}
}

// ���\�[�X�̓ǂݍ���
void Actor3D::Load()
{
	if (m_model)
	{
		m_model->Load();
	}

	// �����蔻��V�X�e���ɓo�^
	if (m_collider)
	{
		ActorCollision3D::GetInstance()->Register(this);
	}
}

// ���\�[�X�̉��
void Actor3D::Release()
{
	if (m_model)
	{
		m_model->Release();
		delete m_model;
		m_model = nullptr;
	}

	// �����蔻��V�X�e�����珜�O
	if (m_collider)
	{
		ActorCollision3D::GetInstance()->Remove(this);
	}

	// �r�b�g�t���O��ێ����Ă���΍폜
	if (m_bitFlag)
	{
		delete m_bitFlag;
		m_bitFlag = nullptr;
	}
}

// �`��
void Actor3D::Draw()
{
	if (m_isActive == false) return;

	if (m_model)
	{
		m_model->Draw(m_transform);
	}

#ifndef NDEBUG
	// DEBUG:��_�̕\��
	DrawLine3D(m_transform.position, m_transform.position + Vector3(3, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(m_transform.position, m_transform.position + Vector3(0, 3, 0), GetColor(0, 255, 0));
	DrawLine3D(m_transform.position, m_transform.position + Vector3(0, 0, 3), GetColor(0, 0, 255));
#endif
}

// �q�m�[�h���܂ލX�V
void Actor3D::TreeUpdate()
{
	// Node�{����TreeUpdate�����s
	Node::TreeUpdate();

	if (m_isActive == false) return;

	// ���f���̍X�V
	if (m_model)
	{
		m_model->Update();
	}
}
