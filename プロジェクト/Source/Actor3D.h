#pragma once
#include "Node.h"
#include "Transform3D.h"
#include "Model.h"
#include <string>

class Collider3D;
class BitFlag;

// Actor�N���X��3D�o�[�W����

class Actor3D : public Node
{
protected:
	std::string m_name;			// �I�u�W�F�N�g��
	Transform3D m_transform;	// �p�����
	Model* m_model;				// 3D�摜�\��
	Collider3D* m_collider;		// �����蔻��
	BitFlag* m_bitFlag;			// ���ʂŎg�p����r�b�g�t���O

	bool m_isActive;

	virtual void Load() override;		// ���\�[�X�̓ǂݍ���
	virtual void Release() override;	// ���\�[�X�̉��
	virtual void Draw() override;		// �`��

public:
	// �R���X�g���N�^
	Actor3D(const char* name, const char* modelName = nullptr, int baseFrameIndex = 0, bool isActive = true);

	// �q�m�[�h���܂ލX�V
	virtual void TreeUpdate() override;

	// �p���̎擾
	const Transform3D& GetTransform() const
	{
		return m_transform;
	}

	// ���W�̎擾
	const Vector3& GetPosition() const
	{
		return m_transform.position;
	}

	// ��]�̎擾
	const Quaternion& GetRotation() const
	{
		return m_transform.rotation;
	}

	// ���O�̎擾
	const std::string& GetName() const
	{
		return m_name;
	}

	// �����蔻��̎擾
	const Collider3D* GetCollider3D() const
	{
		return m_collider;
	}

	// �Փ˃C�x���g
	virtual void OnCollision3D(const Actor3D* other) {}

	// ���f���n���h���̎擾
	const int GetModelHandle() const 
	{
		return m_model->GetModelHandle();
	}

	// �r�b�g�t���O�̎擾
	BitFlag* GetBitFlag() const 
	{
		return m_bitFlag;
	}

	// �A�N�e�B�u�؂�ւ�
	void SetActive(bool state) { m_isActive = state; }

	bool IsActive() { return m_isActive; }
};