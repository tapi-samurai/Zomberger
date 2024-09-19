#include "UiItemFieldBase.h"
#include "SpriteLoader.h"
#include "DxLib.h"
#include "ItemData.h"
#include "Math.h"
#include "ItemField.h"
#include "TextureData.h"

UiItemFieldBase::UiItemFieldBase(int slotHeightNum, int slotWidthNum) :
	m_basePositionTopLeft(Vector2()),
	m_slotHeightNum(slotHeightNum),
	m_slotWidthNum(slotWidthNum),
	m_slotTextureHandle(0),
	m_selectTextureHandle(0),
	m_isActive(false)
{
	// �\������A�C�e���̃t�B�[���h�A�h���X�����񎟌��z����`
	m_itemAddressArray = std::vector<std::vector<int*>>(m_slotHeightNum, std::vector<int*>(m_slotWidthNum, nullptr));

	// �A�C�e���̉摜�n���h�������z����`
	m_itemTextureHandleArray = std::vector<int>(static_cast<int>(ItemData::OtherItem::AllLength), 0);
}

void UiItemFieldBase::Load()
{
	Node::Load();

	// �摜�̃��[�h
	m_slotTextureHandle =
		SpriteLoader::GetInstance()->Load(TextureData::List[static_cast<int>(TextureData::Name::ItemSlot)].TextureName
		);
	m_selectTextureHandle =
		SpriteLoader::GetInstance()->Load(TextureData::List[static_cast<int>(TextureData::Name::SelectSlot)].TextureName
		);

	// �S�ẴA�C�e���摜�����[�h
	for (int itemNum = 0; itemNum < static_cast<int>(ItemData::OtherItem::AllLength); itemNum++)
	{
		m_itemTextureHandleArray[itemNum] = SpriteLoader::GetInstance()->Load(ItemData::List[itemNum].TextureName);
	}
}

void UiItemFieldBase::Release()
{
	Node::Release();

	// �摜�̉��
	SpriteLoader::GetInstance()->Delete(TextureData::List[static_cast<int>(TextureData::Name::ItemSlot)].TextureName);
	SpriteLoader::GetInstance()->Delete(TextureData::List[static_cast<int>(TextureData::Name::SelectSlot)].TextureName);

	// �S�ẴA�C�e���摜���폜
	for (int itemNum = 0; itemNum < static_cast<int>(ItemData::OtherItem::AllLength); itemNum++)
	{
		SpriteLoader::GetInstance()->Delete(ItemData::List[itemNum].TextureName);
	}
}

void UiItemFieldBase::Draw()
{
	Node::Draw();

	if (m_isActive == false) return;

	// �g�̕\��
	for (int heightNum = 0; heightNum < m_slotHeightNum; heightNum++)
	{
		for (int widthNum = 0; widthNum < m_slotWidthNum; widthNum++)
		{
			// �\�����Ȃ��g�ł���΃X�L�b�v
			if (m_itemAddressArray[heightNum][widthNum] == nullptr) continue;

			// �\��������W���v�Z
			Vector2 dispPos = m_basePositionTopLeft + Vector2(widthNum * SlotWidthLength, heightNum * SlotHeightLength);

			// �g��\��
			DrawGraph(static_cast<int>(dispPos.x), static_cast<int>(dispPos.y), m_slotTextureHandle, true);

			// ���g������Ε\��
			int* itemNum = m_itemAddressArray[heightNum][widthNum];
			if (itemNum != nullptr)
			{
				DrawGraph(static_cast<int>(dispPos.x), static_cast<int>(dispPos.y), m_itemTextureHandleArray[*itemNum], true);
			}
		}
	}

	// ���ݑI�����Ă���X���b�g�ɉ摜��\������
	// �J�[�\�����X���b�g�O�ł���Ε\�����Ȃ�
	if (ItemField::GetInstance()->GetSelectSlotNum() == ItemField::OutSlot) return;

	// �\��������W���v�Z
	Vector2 selectSlotNum = ItemField::GetInstance()->GetSelectSlotNum();
	Vector2 arrowDispPos = m_basePositionTopLeft +
		Vector2(selectSlotNum.x * SlotWidthLength, selectSlotNum.y * SlotWidthLength);

	// �摜��\��
	DrawGraph(static_cast<int>(arrowDispPos.x), static_cast<int>(arrowDispPos.y), m_selectTextureHandle, true);
}

// �Z���N�g�g���ړ�(�ړ��ʂ��w��)
void UiItemFieldBase::MoveSelectSlot(const Vector2& input)
{
	if (input == Vector2()) return;

	float moveY = Math::Clamp(input.y, -1.0f, 1.0f);
	int count = 0;

	// �c�ړ�
	while (count < std::abs(input.y))
	{
		// �ړ���̃X���b�g�ԍ����v�Z
		Vector2 selectSlotNum = ItemField::GetInstance()->GetSelectSlotNum();
		Vector2 nextSelectSlotNum = Vector2(selectSlotNum.x, selectSlotNum.y + moveY);

		// �ړ��悪�g�O�ł���ΏI��
		if (nextSelectSlotNum.y < 0 || nextSelectSlotNum.y >= m_slotHeightNum)
		{
			break;
		}

		// �ړ��悪null�ł���΂��̃}�X�̂P�}�X����`�F�b�N����
		if (
			m_itemAddressArray[static_cast<int>(nextSelectSlotNum.y)][static_cast<int>(nextSelectSlotNum.x)] == nullptr
			)
		{
			moveY += moveY;
			continue;
		}

		// �ړ��ʂ����Z�b�g���ă}�X���ړ�
		ItemField::GetInstance()->SetSelectSlotNum(nextSelectSlotNum);
		moveY = Math::Clamp(input.y, -1.0f, 1.0f);
		count++;
	}

	float moveX = Math::Clamp(input.x, -1.0f, 1.0f);
	count = 0;

	// ���ړ�
	while (count < std::abs(input.x))
	{
		// �ړ���̃X���b�g�ԍ����v�Z
		Vector2 selectSlotNum = ItemField::GetInstance()->GetSelectSlotNum();
		Vector2 nextSelectSlotNum = Vector2(selectSlotNum.x + moveX, selectSlotNum.y);

		// �ړ��悪�g�O�ł���ΏI��
		if (nextSelectSlotNum.x < 0 || nextSelectSlotNum.x >= m_slotWidthNum)
		{
			break;
		}

		// �ړ��悪null�ł���΂��̃}�X�̂P�}�X����`�F�b�N����
		int nextSelectSlotNumX = static_cast<int>(nextSelectSlotNum.x);
		int nextSelectSlotNumY = static_cast<int>(nextSelectSlotNum.y);

		if (m_itemAddressArray[static_cast<int>(nextSelectSlotNum.y)][static_cast<int>(nextSelectSlotNum.x)] == nullptr)
		{
			moveX += moveX;
			continue;
		}

		// �ړ��ʂ����Z�b�g���ă}�X���ړ�
		ItemField::GetInstance()->SetSelectSlotNum(nextSelectSlotNum);
		moveX = Math::Clamp(input.x, -1.0f, 1.0f);
		count++;
	}
}

// �Z���N�g�g���ړ�(���ڈړ�����w��)
void UiItemFieldBase::MoveSelectSlot(const int selectHeightNum, const int selectWidthNum)
{
	// �Z���N�g�g���g�O�ł���ΏI��
	if (
		selectHeightNum < 0 || selectHeightNum >= m_slotHeightNum ||
		selectWidthNum < 0 || selectWidthNum >= m_slotWidthNum
		)
	{
		ItemField::GetInstance()->SetSelectSlotNum(ItemField::OutSlot);
		return;
	}

	// �Z���N�g�g�̒��g����`����Ă��Ȃ���ΏI��
	if (m_itemAddressArray[selectHeightNum][selectWidthNum] == nullptr)
	{
		ItemField::GetInstance()->SetSelectSlotNum(ItemField::OutSlot);
		return;
	}

	ItemField::GetInstance()->SetSelectSlotNum(Vector2(selectWidthNum, selectHeightNum));
}

// �\���ؑ�
const void UiItemFieldBase::SetActive(bool value)
{
	m_isActive = value;

	// �\���ؑ֎��ɕK���Z���N�g�g��(0, 0)�ɖ߂�
	MoveSelectSlot(0, 0);

	// �͂�ł���A�C�e���𗣂�����
	ReleaseItem();
}