#pragma once
#include "Vector2.h"
#include <vector>

class ItemField
{
public:
	// �A�C�e���t�B�[���h���ʗp�̗񋓑�
	enum class FieldName
	{
		Inventory,
		Itembox,
		HamburgerRecipe,
		Trader,

		Length,
	};

	static constexpr int ItemFieldSize[static_cast<int>(FieldName::Length)] = { 5, 10, 5, 1 };
	static constexpr Vector2 OutSlot = Vector2(-1, -1);	// �g�O��\��

private:
	std::vector<std::vector<int>> m_itemField;	// �A�C�e���f�[�^�z��

	Vector2 m_selectSlotNum;	// �Z���N�g�g�ԍ�

public:
	// �V���O���g��
	static ItemField* GetInstance()
	{
		static ItemField instance;
		return &instance;
	}

	ItemField();

	// �A�C�e�����̏�����
	void Initialize();

	// �w��g�ɓ����Ă���A�C�e���ԍ����擾
	int CheckInventory(FieldName name, int num) { return m_itemField[static_cast<int>(name)][num]; }

	// �A�C�e����ǉ�(����:true ���s:false)
	bool AddItem(FieldName name, const int itemNum);

	// �w��g�̃A�C�e����Null�ɕύX
	void ReduceItem(FieldName name, int widthNum);

	// �w��g�̃A�h���X���擾
	int* GetItemFieldAddress(FieldName name, int widthNum) { return &m_itemField[static_cast<int>(name)][widthNum]; }

	// �Z���N�g�g��ݒ�
	void SetSelectSlotNum(const Vector2& slot) { m_selectSlotNum = slot; }

	// �Z���N�g�g�ԍ����擾
	const Vector2& GetSelectSlotNum() const { return m_selectSlotNum; }
};