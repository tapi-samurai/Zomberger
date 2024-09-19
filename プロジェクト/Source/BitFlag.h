#pragma once

// �r�b�g�̕ێ��Ƒ���(�r�b�g�̒��g�͔c�����Ȃ�)

class BitFlag
{
private:
	char m_bitFlag;

public:
	BitFlag() :
		m_bitFlag(0) {}

	// ������
	const void Initialize() { m_bitFlag = 0; }

	// �w��̃r�b�g�̊m�F
	const bool IsFlag(int flag) { return (m_bitFlag & (1 << flag)) != 0; }

	// �w��̃r�b�g��true�ɂ���
	const void OnFlag(int flag) { m_bitFlag |= (1 << flag); }

	// �w��̃r�b�g��false�ɂ���
	const void OffFlag(int flag) { m_bitFlag &= (~(1 << flag)); }
};