#include "UiInteractItemGage.h"
#include "TextureData.h"
#include "SpriteLoader.h"
#include "TextureData.h"
#include "Time.h"
#include "Math.h"

UiInteractItemGage::UiInteractItemGage() :
	m_keyGraphHandleArray(std::vector<int>(2)),
	m_position(VECTOR()),
	m_gageRate(0),
	m_elapsedTime(0),
	m_isDisp(false),
	m_keyBackgroundHandle(0)
{
	// �摜��o�^
	m_neutralKey.Register(TextureData::List[static_cast<int>(TextureData::Name::KeyNeutralF)].TextureName);
	m_pressKey.Register(TextureData::List[static_cast<int>(TextureData::Name::KeyPressF)].TextureName);
	m_keyBackground.Register(TextureData::List[static_cast<int>(TextureData::Name::KeyBackGround)].TextureName);

	// �摜�̃T�C�Y��ݒ�
	m_neutralKey.gridSize = Vector2(KeyWidth, KeyHeight);
	m_pressKey.gridSize = Vector2(KeyWidth, KeyHeight);
	m_keyBackground.gridSize = Vector2(KeyWidth, KeyHeight);
};

void UiInteractItemGage::Load()
{
	Node::Load();

	m_neutralKey.Load();
	m_pressKey.Load();
	m_keyBackground.Load();
}

void UiInteractItemGage::Release()
{
	Node::Release();

	m_neutralKey.Release();
	m_pressKey.Release();
	m_keyBackground.Release();
}

void UiInteractItemGage::Update()
{
	Node::Update();

	m_neutralKey.Update();
	m_pressKey.Update();
	m_keyBackground.Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// �l���傫���Ȃ肷����̂�h��
	if (m_elapsedTime >= KeyBlinkingDelay * 2)
	{
		m_elapsedTime = 0;
	}
}

void UiInteractItemGage::Draw()
{
	Node::Draw();

	// �\���t���O�������ł���΃X�L�b�v
	if (m_isDisp == false) return;

	// �g��\��
	DrawBox(
		static_cast<int>(m_position.x) - FrameWidth / 2, static_cast<int>(m_position.y) - FrameHeight / 2,
		static_cast<int>(m_position.x) + FrameWidth / 2, static_cast<int>(m_position.y) + FrameHeight / 2,
		GetColor(255, 255, 255), false
	);

	// �Q�[�W��\��
	DrawBox(
		static_cast<int>(m_position.x) - GageWidth / 2,
		static_cast<int>(m_position.y) - GageHeight / 2,
		static_cast<int>(m_position.x) - GageWidth / 2 + static_cast<int>(GageWidth * std::sin((m_gageRate * Math::Fpi) / 2)),
		static_cast<int>(m_position.y) + GageHeight / 2,
		GetColor(87, 202, 255), true
	);

	// �L�[��\��
	Vector2 position = Vector2(m_position.x, m_position.y) + KeyPositionOffset;
	m_keyBackground.Draw(position, Sprite::DrawMode::Extend);

	// �o�ߎ��Ԃɍ��킹�ă{�^����_�ŕ\��
	if (m_elapsedTime < KeyBlinkingDelay)
	{
		m_neutralKey.Draw(position, Sprite::DrawMode::Extend);
	}
	else
	{
		m_pressKey.Draw(position, Sprite::DrawMode::Extend);
	}

	// �\���t���O�𖳌���
	m_isDisp = false;
}

void UiInteractItemGage::OnDisp(const VECTOR& position, const float rate)
{
	m_isDisp = true;
	m_position = position;
	m_gageRate = rate;
}

// �C���^���N�g���ɖ��t���[���Ă΂��
void UiInteractItemGage::IsInteract()
{
	// �o�ߎ��Ԃ��{�^���������ɌŒ�
	m_elapsedTime = KeyPressElapsedTime;
}