#include "Button.h"
#include "Input.h"

bool Button::IsMouseContain(const Vector2& position) const
{
	// 矩形(ボタン)に点(マウス)が含まれているか
	Vector2 mousePoint = Input::GetInstance()->GetMousePoint();
	if (position.x - m_size.x / 2 <= mousePoint.x && mousePoint.x <= position.x + m_size.x / 2
		&& position.y - m_size.y / 2 <= mousePoint.y && mousePoint.y <= position.y + m_size.y / 2)
	{
		return true;
	}
	return false;
}

void Button::Update(const Vector2& position)
{
	// ボタン上でマウスボタンを押下した上で、ボタン上でマウスボタンを離したらボタンが押されたことにする

	// ボタン上で指定されたマウスボタンが押されたか
	if (Input::GetInstance()->IsMouseDown(m_mouseButton))
	{
		m_isClickDown = IsMouseContain(position);
	}

	// ボタン上で指定されたマウスボタンが離したか
	if (m_isClickDown && Input::GetInstance()->IsMouseUp(m_mouseButton))
	{
		m_isClickDown = false;
		if (IsMouseContain(position))
		{
			// コールバック関数呼び出し
			m_callback();
		}
	}
}

void Button::Draw(const Vector2& position)
{
	DrawBox(
		static_cast<int>(position.x - m_size.x / 2), static_cast<int>(position.y - m_size.y / 2),
		static_cast<int>(position.x + m_size.x / 2), static_cast<int>(position.y + m_size.y / 2),
		GetColor(0, 0, 255), 0);
}