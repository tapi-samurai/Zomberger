#pragma once

// ビットの保持と操作(ビットの中身は把握しない)

class BitFlag
{
private:
	char m_bitFlag;

public:
	BitFlag() :
		m_bitFlag(0) {}

	// 初期化
	const void Initialize() { m_bitFlag = 0; }

	// 指定のビットの確認
	const bool IsFlag(int flag) { return (m_bitFlag & (1 << flag)) != 0; }

	// 指定のビットをtrueにする
	const void OnFlag(int flag) { m_bitFlag |= (1 << flag); }

	// 指定のビットをfalseにする
	const void OffFlag(int flag) { m_bitFlag &= (~(1 << flag)); }
};