#include "ItemField.h"
#include "DxLib.h"
#include "ItemData.h"

ItemField::ItemField()
{
	// �A�C�e���̃f�[�^�����񎟌��z����`
	m_itemField = std::vector<std::vector<int>>(static_cast<int>(FieldName::Length));

	for (int itemFieldNum = 0; itemFieldNum < static_cast<int>(FieldName::Length); itemFieldNum++)
	{
		// �w��̗v�f���ɕύX
		m_itemField[itemFieldNum].resize(ItemFieldSize[itemFieldNum]);
	}
}

// �A�C�e�����̏�����
void ItemField::Initialize()
{
	for (int fieldNameNum = 0; fieldNameNum < static_cast<int>(FieldName::Length); fieldNameNum++)
	{
		for (int widthNum = 0; widthNum < ItemFieldSize[fieldNameNum]; widthNum++)
		{
			ReduceItem(static_cast<FieldName>(fieldNameNum), widthNum);
		}
	}
}

// �C���x���g���̍ł�0�ɋ߂���̏ꏊ�ɃA�C�e����ǉ�
bool ItemField::AddItem(FieldName name, int itemNum)
{
	int itemFieldType = static_cast<int>(name);

	for (int itemFieldNum = 0; itemFieldNum < ItemFieldSize[itemFieldType]; itemFieldNum++)
	{
		if (m_itemField[itemFieldType][itemFieldNum] == static_cast<int>(ItemData::RecipeItem::Null))
		{
			m_itemField[itemFieldType][itemFieldNum] = itemNum;

			return true;
		}
	}

	return false;
}

// �w��̘g�̃A�C�e������Null�ɕύX
void ItemField::ReduceItem(FieldName name, int widthNum)
{
	m_itemField[static_cast<int>(name)][widthNum] = static_cast<int>(ItemData::RecipeItem::Null);
}