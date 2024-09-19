#include "ItemField.h"
#include "DxLib.h"
#include "ItemData.h"

ItemField::ItemField()
{
	// アイテムのデータを持つ二次元配列を定義
	m_itemField = std::vector<std::vector<int>>(static_cast<int>(FieldName::Length));

	for (int itemFieldNum = 0; itemFieldNum < static_cast<int>(FieldName::Length); itemFieldNum++)
	{
		// 指定の要素数に変更
		m_itemField[itemFieldNum].resize(ItemFieldSize[itemFieldNum]);
	}
}

// アイテム情報の初期化
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

// インベントリの最も0に近い空の場所にアイテムを追加
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

// 指定の枠のアイテム情報をNullに変更
void ItemField::ReduceItem(FieldName name, int widthNum)
{
	m_itemField[static_cast<int>(name)][widthNum] = static_cast<int>(ItemData::RecipeItem::Null);
}