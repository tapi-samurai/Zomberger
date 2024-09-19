#include "Button.h"
#include "Input.h"

bool Button::IsMouseContain(const Vector2& position) const
{
	// ��`(�{�^��)�ɓ_(�}�E�X)���܂܂�Ă��邩
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
	// �{�^����Ń}�E�X�{�^��������������ŁA�{�^����Ń}�E�X�{�^���𗣂�����{�^���������ꂽ���Ƃɂ���

	// �{�^����Ŏw�肳�ꂽ�}�E�X�{�^���������ꂽ��
	if (Input::GetInstance()->IsMouseDown(m_mouseButton))
	{
		m_isClickDown = IsMouseContain(position);
	}

	// �{�^����Ŏw�肳�ꂽ�}�E�X�{�^������������
	if (m_isClickDown && Input::GetInstance()->IsMouseUp(m_mouseButton))
	{
		m_isClickDown = false;
		if (IsMouseContain(position))
		{
			// �R�[���o�b�N�֐��Ăяo��
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