#pragma once
#include "Vector2.h"
#include <vector>

class ItemField
{
public:
	// アイテムフィールド識別用の列挙体
	enum class FieldName
	{
		Inventory,
		Itembox,
		HamburgerRecipe,
		Trader,

		Length,
	};

	static constexpr int ItemFieldSize[static_cast<int>(FieldName::Length)] = { 5, 10, 5, 1 };
	static constexpr Vector2 OutSlot = Vector2(-1, -1);	// 枠外を表す

private:
	std::vector<std::vector<int>> m_itemField;	// アイテムデータ配列

	Vector2 m_selectSlotNum;	// セレクト枠番号

public:
	// シングルトン
	static ItemField* GetInstance()
	{
		static ItemField instance;
		return &instance;
	}

	ItemField();

	// アイテム情報の初期化
	void Initialize();

	// 指定枠に入っているアイテム番号を取得
	int CheckInventory(FieldName name, int num) { return m_itemField[static_cast<int>(name)][num]; }

	// アイテムを追加(成功:true 失敗:false)
	bool AddItem(FieldName name, const int itemNum);

	// 指定枠のアイテムをNullに変更
	void ReduceItem(FieldName name, int widthNum);

	// 指定枠のアドレスを取得
	int* GetItemFieldAddress(FieldName name, int widthNum) { return &m_itemField[static_cast<int>(name)][widthNum]; }

	// セレクト枠を設定
	void SetSelectSlotNum(const Vector2& slot) { m_selectSlotNum = slot; }

	// セレクト枠番号を取得
	const Vector2& GetSelectSlotNum() const { return m_selectSlotNum; }
};