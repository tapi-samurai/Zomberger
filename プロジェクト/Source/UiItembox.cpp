#include "UiItembox.h"
#include "ItemField.h"
#include "ItemData.h"
#include "Input.h"
#include "Screen.h"
#include "SoundLoader.h"
#include "SoundData.h"
#include "SoundEffect.h"

UiItembox::UiItembox() :
	UiItemFieldBase(HeightNum, WidthNum),
	m_selectItemNum(static_cast<int>(ItemData::RecipeItem::Null)),
	m_seClick(0)
{
	// �z��ɃC���x���g���̃A�h���X��ǉ�

	// �A�C�e���{�b�N�X��o�^
	// �c��2�~5
	for (
		int itemboxNum = 0;
		itemboxNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::Itembox)];
		itemboxNum++
		)
	{
		int itemAddressArrayHeightNum = itemboxNum / WidthNum;
		int itemAddressArrayWidthNum = itemboxNum % WidthNum;

		m_itemAddressArray[itemAddressArrayHeightNum][itemAddressArrayWidthNum] =
			ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Itembox, itemboxNum);
	}

	// �C���x���g��
	// �ŉ��i
	for (
		int inventoryNum = 0;
		inventoryNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::Inventory)];
		inventoryNum++
		)
	{
		m_itemAddressArray[m_itemAddressArray.size() - 1][inventoryNum] =
			ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Inventory, inventoryNum);
	}

	// ��_���W��ݒ�
	m_basePositionTopLeft = BasePosition;
}

void UiItembox::Load()
{
	UiItemFieldBase::Load();

	// SE�ǂݍ���
	m_seClick = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seClick, SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].volume);
}

void UiItembox::Release()
{
	UiItemFieldBase::Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
}

void UiItembox::Update()
{
	Node::Update();

	if (IsActive() == false) return;
	
	// �}�E�X�̍��W����_�ɑ΂��郍�[�J�����W�ɕϊ�
	Vector2 mousePos = Input::GetInstance()->GetMousePoint();
	Vector2 localMousePos = Vector2(mousePos.x - BasePosition.x, mousePos.y - BasePosition.y);

	// �}�E�X�̍��W���X���b�g�ԍ��ɕϊ�
	int selectHeightNum = static_cast<int>(localMousePos.y / SlotHeightLength + (localMousePos.y < 0 ? -1 : 0));
	int selectWidthNum = static_cast<int>(localMousePos.x / SlotWidthLength + (localMousePos.x < 0 ? -1 : 0));

	// �Z���N�g�g���ړ�
	MoveSelectSlot(selectHeightNum, selectWidthNum);

	// �A�C�e���̈ړ�
	if (
		ItemField::GetInstance()->GetSelectSlotNum() != ItemField::OutSlot &&
		Input::GetInstance()->IsMouseDown(MOUSE_INPUT_LEFT)
		)
	{
		int selectHeight = static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().y);
		int selectWidth = static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x);

		// ���ݒ͂�ł���A�C�e���ƃZ���N�g�g�̃A�C�e����؂�ւ���
		int selectItemBaseCopy = m_selectItemNum;
		m_selectItemNum = *m_itemAddressArray[selectHeight][selectWidth];
		*m_itemAddressArray[selectHeight][selectWidth] = selectItemBaseCopy;

		// SE
		PlaySoundMem(m_seClick, DX_PLAYTYPE_BACK);
	}
}

void UiItembox::Draw()
{
	if (IsActive() == false) return;

	UiItemFieldBase::Draw();

	// ���ݒ͂�ł���A�C�e�����}�E�X�J�[�\���̈ʒu�ɕ\��
	if (m_selectItemNum != static_cast<int>(ItemData::RecipeItem::Null))
	{
		int dispPosX = static_cast<int>(Input::GetInstance()->GetMousePoint().x - SlotWidthLength / 2.0f);
		int dispPosY = static_cast<int>(Input::GetInstance()->GetMousePoint().y - SlotHeightLength / 2.0f);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[m_selectItemNum], true);
	}
}

void UiItembox::ReleaseItem()
{
	if (m_selectItemNum == static_cast<int>(ItemData::RecipeItem::Null)) return;

	// �A�C�e�����ڂ���ꏊ��T��
	for (int height = 0; height < HeightNum; height++)
	{
		for (int width = 0; width < WidthNum; width++)
		{
			if (
				m_itemAddressArray[height][width] == nullptr ||
				*m_itemAddressArray[height][width] != static_cast<int>(ItemData::RecipeItem::Null)
				)
			{
				continue;
			}

			// �A�C�e�����ڂ�
			*m_itemAddressArray[height][width] = m_selectItemNum;

			// �͂�ł���A�C�e����Null�ɂ���
			m_selectItemNum = static_cast<int>(ItemData::RecipeItem::Null);
		}
	}
}