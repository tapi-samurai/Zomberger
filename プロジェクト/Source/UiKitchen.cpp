#include "UiKitchen.h"
#include "ItemData.h"
#include "ItemField.h"
#include "input.h"
#include "SpriteLoader.h"
#include "Button.h"
#include "Sprite.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "TextureData.h"

UiKitchen::UiKitchen() :
	UiItemFieldBase(SlotHeightNum, SlotWidthNum),
	m_cookButton(Button(Vector2(ButtonWidth, ButtonHeight), MOUSE_INPUT_LEFT, std::bind(&UiKitchen::OnClick, this))),
	m_selectItemNum(static_cast<int>(ItemData::RecipeItem::Null)),
	m_seClick(0),
	m_seCook(0)
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
		int itemAddressArrayHeightNum = itemboxNum / ItemboxWidth + 1;
		int itemAddressArrayWidthNum = itemboxNum % ItemboxWidth;

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

	// �n���o�[�K�[�X���b�g
	// �E���
	for (
		int hamburgerSlotNum = 0;
		hamburgerSlotNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
		hamburgerSlotNum++
		)
	{
		m_itemAddressArray[hamburgerSlotNum][SlotWidthNum - 1] =
			ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::HamburgerRecipe, hamburgerSlotNum);
	}

	// ��_���W��ݒ�
	m_basePositionTopLeft = BasePosition;

	// �{�^���摜��o�^
	m_spriteButton = new Sprite();
	m_spriteButton->Register(TextureData::List[static_cast<int>(TextureData::Name::CookButton)].TextureName);
}

void UiKitchen::Load()
{
	UiItemFieldBase::Load();

	m_spriteButton->Load();

	// SE�ǂݍ���
	m_seClick = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
	m_seCook = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Cook)].SoundName);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seClick, SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].volume);
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seCook, SoundData::List[static_cast<int>(SoundData::Name::Cook)].volume);
}

void UiKitchen::Release()
{
	UiItemFieldBase::Release();

	m_spriteButton->Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
}

void UiKitchen::Update()
{
	Node::Update();

	if (IsActive() == false) return;

	m_cookButton.Update(ButtonPosition);
	m_spriteButton->Update();

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

void UiKitchen::Draw()
{
	if (IsActive() == false) return;

	m_cookButton.Draw(ButtonPosition);
	m_spriteButton->Draw(ButtonPosition);

	UiItemFieldBase::Draw();

	// �`�悷��摜�̖��邳���Â�����
	SetDrawBright(HintItemBright, HintItemBright, HintItemBright);

	// �n���o�[�K�[�ɕK�v�ȃA�C�e�����{�b�N�X�ɈÂ��\��
	for (
		int hamburgerSlotNum = 0;
		hamburgerSlotNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
		hamburgerSlotNum++
		)
	{
		// ���ɒ��g���Z�b�g����Ă�����`�悵�Ȃ�
		if (*m_itemAddressArray[hamburgerSlotNum][SlotWidthNum - 1] != static_cast<int>(ItemData::RecipeItem::Null)) continue;

		int dispPosX = static_cast<int>(m_basePositionTopLeft.x + (SlotWidthNum - 1) * SlotWidthLength);
		int dispPosY = static_cast<int>(m_basePositionTopLeft.y + hamburgerSlotNum * SlotHeightLength);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[hamburgerSlotNum + 1], true);
	}

	// �`�悷��摜�̖��邳��W���ɖ߂�
	SetDrawBright(255, 255, 255);

	// ���ݒ͂�ł���A�C�e�����}�E�X�J�[�\���̈ʒu�ɕ\��
	if (m_selectItemNum != static_cast<int>(ItemData::RecipeItem::Null))
	{
		int dispPosX = static_cast<int>(Input::GetInstance()->GetMousePoint().x - SlotWidthLength / 2.0f);
		int dispPosY = static_cast<int>(Input::GetInstance()->GetMousePoint().y - SlotHeightLength / 2.0f);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[m_selectItemNum], true);
	}
}

void UiKitchen::OnClick()
{
	// �n���o�[�K�[�̃��V�s�������Ă��邩�`�F�b�N
	for (
		int hamburgerNum = 0;
		hamburgerNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
		hamburgerNum++
		)
	{
		// ���V�s�������Ă��Ȃ���΃X�L�b�v
		if (*m_itemAddressArray[hamburgerNum][SlotWidthNum - 1] != hamburgerNum + 1) return;
	}

	// ���ׂđ����Ă�����C���x���g���Ƀn���o�[�K�[��ǉ�
	int hamburger = static_cast<int>(ItemData::OtherItem::Hamburger);
	if (
		ItemField::GetInstance()->AddItem(
			ItemField::FieldName::Itembox, hamburger)
		)
	{
		// �ǉ��o�����烌�V�s���̃A�C�e����S����
		for (
			int widthNum = 0;
			widthNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
			widthNum++
			)
		{
			ItemField::GetInstance()->ReduceItem(ItemField::FieldName::HamburgerRecipe, widthNum);
		}
	}

	// SE
	PlaySoundMem(m_seCook, DX_PLAYTYPE_BACK);
}

void UiKitchen::ReleaseItem()
{
	if (m_selectItemNum == static_cast<int>(ItemData::RecipeItem::Null)) return;

	// �A�C�e�����ڂ���ꏊ��T��
	for (int height = 0; height < SlotHeightNum; height++)
	{
		for (int width = 0; width < SlotWidthNum; width++)
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