#pragma once
#include "Node.h"
#include "DxLib.h"
#include "Sprite.h"
#include <vector>

class UiInteractable : public Node
{
private:
	static constexpr float KeyBlinkingDelay = 0.5f;		// ボタンの点滅間隔
	static constexpr int KeyHeight = 32;
	static constexpr int KeyWidth = 32;
	static constexpr float KeyPressElapsedTime = 0.95f;	// ボタン押下時に経過時間に代入する値

	Sprite m_neutralKey;
	Sprite m_pressKey;
	Sprite m_keyBackground;

	std::vector<int> m_keyGraphHandleArray;
	int m_keyBackgroundHandle;
	VECTOR m_position;
	float m_elapsedTime;
	bool m_isDisp;

protected:
	void Load() override;
	void Release() override;
	void Update() override;
	void Draw() override;

public:
	UiInteractable();

	void OnDisp(const VECTOR& position);
	void IsInteract();
};