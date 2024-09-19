#pragma once
#include "Node.h"
#include "Vector2.h"
#include <vector>

class UiItemFieldBase : public Node
{
private:
	int m_slotHeightNum;		// �c�̃X���b�g��(�\�����Ȃ��󔒂��܂�)
	int m_slotWidthNum;			// �c�̃X���b�g��(�\�����Ȃ��󔒂��܂�)
	int m_slotTextureHandle;	// �X���b�g�̉摜�n���h��
	int m_selectTextureHandle;	// �Z���N�g�g�̉摜�n���h��

	bool m_isActive;

protected:
	static constexpr int SlotHeightLength = 64;	// �X���b�g�̏c�̕�
	static constexpr int SlotWidthLength = 64;	// �X���b�g�̉��̕�

	Vector2 m_basePositionTopLeft;		// �\���̊�_���W(����)

	std::vector<std::vector<int*>> m_itemAddressArray;	// �\������A�C�e���̃t�B�[���h�A�h���X�����񎟌��z��
	std::vector<int> m_itemTextureHandleArray;			// �A�C�e���̕\���Ɏg�p����e�N�X�`���̔z��

	void Load() override;
	void Release() override;
	void Draw() override;

	virtual void Update() override = 0;
	virtual void ReleaseItem() {}

public:
	UiItemFieldBase(int slotHeightNum, int slotWidthNum);

	void MoveSelectSlot(const Vector2& input);									// �Z���N�g�g���ړ�(�ړ��ʂ��w��)
	void MoveSelectSlot(const int selectHeightNum, const int selectWidthNum);	// �Z���N�g�g���ړ�(���ڈړ�����w��)

	const void SetActive(bool value);
	const bool IsActive() const { return m_isActive; }
};