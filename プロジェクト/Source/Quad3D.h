#pragma once
#include "Actor3D.h"

// �����蔻��Ɖ摜�̕\���@�\������Actor3D�p���N���X

class Quad3D : public Actor3D
{
private:
	static constexpr int VertexNum = 4;	// �O�p�`�|���S�����`�����钸�_�̐�
	static constexpr int IndexNum = 6;	// ���_�C���f�b�N�X�̐�

	// ���_�C���f�b�N�X�萔�z��
	static constexpr WORD Index[IndexNum] =
	{
		0,1,2,0,3,2,
	};		

	VERTEX3D m_vertex[VertexNum];	// �O�p�`�|���S�����`�����钸�_�z��

	const char* m_textureName;

	float m_height;
	float m_width;

	int m_textureHandle;

	bool m_isDisplay;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Quad3D(
		const Vector3& position, const Quaternion& rotation, float height, float width,
		const char* name, const char* textureName = nullptr, bool isDisplay = true, const COLOR_U8& color = GetColorU8(255, 255, 255, 255)
		);

	const void SetPosition (const Vector3& position) { m_transform.position = position; }
};