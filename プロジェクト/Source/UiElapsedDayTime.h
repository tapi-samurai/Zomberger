#pragma once
#include "Node.h"

class UiElapsedDayTime : public Node
{
private:
	static constexpr int Width = 192;				// �摜��\�����鉡��
	static constexpr int BaseX = 120;				// ��_X���W
	static constexpr int BaseY = 110;				// ��_Y���W
	static constexpr int DestX = BaseX - Width / 2;	// �J�����_�[��\������X���W
	static constexpr int DestY = BaseY - Width / 2;	// �J�����_�[��\������Y���W
	static constexpr int NumY = 62;					// ������\������Y���W

	int m_calendarHandle;
	int m_fontHandle;

protected:
	void Load() override;
	void Release() override;
	void Draw() override;

public:
	UiElapsedDayTime() :
		m_calendarHandle(0),
		m_fontHandle(0) {};
};