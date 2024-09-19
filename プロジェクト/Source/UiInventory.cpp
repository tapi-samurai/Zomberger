#include "UiInventory.h"
#include "ItemField.h"
#include "Input.h"
#include "Math.h"
#include "ItemData.h"

UiInventory::UiInventory() :
	UiItemFieldBase(HeightNum, WidthNum)
{
	// �z��ɃC���x���g���̃A�h���X��o�^
	for (
		int inventoryNum = 0;
		inventoryNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::Inventory)];
		inventoryNum++
		)
	{
		m_itemAddressArray[0][inventoryNum] = ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Inventory, inventoryNum);
	}

	// ��_���W��ݒ�
	m_basePositionTopLeft = BasePosition;

	// �\��
	SetActive(true);
}

void UiInventory::Update()
{
	if (IsActive() == false) return;

	Node::Update();

	// ���͂��擾
	float mouseWheel = 0;
	mouseWheel = -static_cast<float>(Input::GetInstance()->GetMouseWheelRot());

	// ���E�̒[���ړ��ł���悤�ɂ��鏈��
	Vector2 vec = ItemField::GetInstance()->GetSelectSlotNum();
	int currentSelectSlotNumX = Math::Clamp(static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x), 0, WidthNum - 1);
	mouseWheel =
		static_cast<float>((currentSelectSlotNumX + static_cast<int>(mouseWheel) + WidthNum) % WidthNum - currentSelectSlotNumX);

	// �Z���N�g�g���ړ�
	MoveSelectSlot(Vector2(mouseWheel, 0));
}