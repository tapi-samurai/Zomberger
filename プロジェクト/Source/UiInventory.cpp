#include "UiInventory.h"
#include "ItemField.h"
#include "Input.h"
#include "Math.h"
#include "ItemData.h"

UiInventory::UiInventory() :
	UiItemFieldBase(HeightNum, WidthNum)
{
	// 配列にインベントリのアドレスを登録
	for (
		int inventoryNum = 0;
		inventoryNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::Inventory)];
		inventoryNum++
		)
	{
		m_itemAddressArray[0][inventoryNum] = ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Inventory, inventoryNum);
	}

	// 基点座標を設定
	m_basePositionTopLeft = BasePosition;

	// 表示
	SetActive(true);
}

void UiInventory::Update()
{
	if (IsActive() == false) return;

	Node::Update();

	// 入力を取得
	float mouseWheel = 0;
	mouseWheel = -static_cast<float>(Input::GetInstance()->GetMouseWheelRot());

	// 左右の端を移動できるようにする処理
	Vector2 vec = ItemField::GetInstance()->GetSelectSlotNum();
	int currentSelectSlotNumX = Math::Clamp(static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x), 0, WidthNum - 1);
	mouseWheel =
		static_cast<float>((currentSelectSlotNumX + static_cast<int>(mouseWheel) + WidthNum) % WidthNum - currentSelectSlotNumX);

	// セレクト枠を移動
	MoveSelectSlot(Vector2(mouseWheel, 0));
}