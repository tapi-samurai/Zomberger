#include "UiTrader.h"
#include "Button.h"
#include "DxLib.h"
#include "ItemData.h"
#include "ItemField.h"
#include "Sprite.h"
#include "Input.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "TextureData.h"

UiTrader::UiTrader() :
	UiItemFieldBase(SlotHeightNum, SlotWidthNum),
	m_tradeButton(Button(Vector2(ButtonWidth, ButtonHeight), MOUSE_INPUT_LEFT, std::bind(&UiTrader::OnClick, this))),
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
		int itemAddressArrayHeightNum = itemboxNum / ItemboxWidth;
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

	// �����g
	// �E��
	m_itemAddressArray[m_itemAddressArray.size() - 1][m_itemAddressArray[0].size() - 1] =
		ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Trader, 0);

	// ��_���W��ݒ�
	m_basePositionTopLeft = BasePosition;

	// �{�^���摜��o�^
	m_spriteButton = new Sprite();
	m_spriteButton->Register(TextureData::List[static_cast<int>(TextureData::Name::TradeButton)].TextureName);
}

void UiTrader::Load()
{
	UiItemFieldBase::Load();

	m_spriteButton->Load();

	// SE�ǂݍ���
	m_seClick = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seClick, SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].volume);
}

void UiTrader::Release()
{
	UiItemFieldBase::Release();

	m_spriteButton->Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
}

void UiTrader::Update()
{
	Node::Update();

	if (IsActive() == false) return;

	// �{�^���̍X�V����
	m_tradeButton.Update(ButtonPosition);
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

void UiTrader::Draw()
{
	if (IsActive() == false) return;

	// �{�^���̕`�揈��
	m_tradeButton.Draw(ButtonPosition);
	m_spriteButton->Draw(ButtonPosition);

	UiItemFieldBase::Draw();

	// �`�悷��摜�̖��邳���Â�����
	SetDrawBright(HintItemBright, HintItemBright, HintItemBright);

	// �����ɕK�v�ȃA�C�e�����{�b�N�X�ɈÂ��\��
	if (
		*m_itemAddressArray[m_itemAddressArray.size() - 1][m_itemAddressArray[m_itemAddressArray.size() - 1].size() - 1] ==
		static_cast<int>(ItemData::RecipeItem::Null)
		)
	{
		int dispPosX =
			static_cast<int>(m_basePositionTopLeft.x + (m_itemAddressArray[m_itemAddressArray.size() - 1].size() - 1) * SlotWidthLength);
		int dispPosY =
			static_cast<int>(m_basePositionTopLeft.y + (m_itemAddressArray.size() - 1) * SlotHeightLength);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[static_cast<int>(ItemData::OtherItem::Hamburger)], true);
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

void UiTrader::OnClick()
{
	// �����ɕK�v�ȃA�C�e���������Ă��Ȃ���΃X�L�b�v
	if (
		*m_itemAddressArray[m_itemAddressArray.size() - 1][m_itemAddressArray[m_itemAddressArray.size() - 1].size() - 1] !=
		static_cast<int>(ItemData::OtherItem::Hamburger)
		)
	{
		return;
	}

	// �A�C�e���������Ă���ꍇ�̓X�R�b�v�ɒu��������
	ItemField::GetInstance()->ReduceItem(ItemField::FieldName::Trader, 0);
	ItemField::GetInstance()->AddItem(ItemField::FieldName::Trader, static_cast<int>(ItemData::OtherItem::Scoop));
}