#include "UiInteractable.h"
#include "SpriteLoader.h"
#include "TextureData.h"
#include "Time.h"
#include "Vector2.h"
#include "Sprite.h"

UiInteractable::UiInteractable() :
	m_keyGraphHandleArray(std::vector<int>(2)),
	m_position(VECTOR()),
	m_elapsedTime(0),
	m_isDisp(false),
	m_keyBackgroundHandle(0)
{
	// 画像の登録
	m_neutralKey.Register(TextureData::List[static_cast<int>(TextureData::Name::KeyNeutralF)].TextureName);
	m_pressKey.Register(TextureData::List[static_cast<int>(TextureData::Name::KeyPressF)].TextureName);
	m_keyBackground.Register(TextureData::List[static_cast<int>(TextureData::Name::KeyBackGround)].TextureName);

	m_neutralKey.gridSize = Vector2(KeyHeight, KeyWidth);
	m_pressKey.gridSize = Vector2(KeyHeight, KeyWidth);
	m_keyBackground.gridSize = Vector2(KeyHeight, KeyWidth);
}

void UiInteractable::Load()
{
	Node::Load();

	// 画像のロード
	m_neutralKey.Load();
	m_pressKey.Load();
	m_keyBackground.Load();
}

void UiInteractable::Release()
{
	Node::Release();

	// 画像の解放
	m_neutralKey.Release();
	m_pressKey.Release();
	m_keyBackground.Release();
}

void UiInteractable::Update()
{
	Node::Update();

	m_neutralKey.Update();
	m_pressKey.Update();
	m_keyBackground.Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// 値が大きくなりすぎるのを防ぐ
	if (m_elapsedTime >= KeyBlinkingDelay * 2)
	{
		m_elapsedTime = 0;
	}
}

void UiInteractable::Draw()
{
	Node::Draw();

	// 表示フラグが無効であればスキップ
	if (m_isDisp == false) return;

	// 画像を表示
	Vector2 position = Vector2(m_position.x, m_position.y);
	m_keyBackground.Draw(position, Sprite::DrawMode::Extend);

	if (m_elapsedTime < KeyBlinkingDelay)
	{
		m_neutralKey.Draw(position, Sprite::DrawMode::Extend);
	}
	else
	{
		m_pressKey.Draw(position, Sprite::DrawMode::Extend);
	}

	// 表示フラグを無効化
	m_isDisp = false;
}

void UiInteractable::OnDisp(const VECTOR& position)
{
	m_isDisp = true;
	m_position = position;
}

// インタラクト時に毎フレーム呼ばれる
void UiInteractable::IsInteract()
{
	// 経過時間をボタン押下時に固定
	m_elapsedTime = KeyPressElapsedTime;
}