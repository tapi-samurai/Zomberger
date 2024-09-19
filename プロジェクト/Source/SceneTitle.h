#pragma once
#include "SceneBase.h"
#include "Vector3.h"

class Node;

// クリアシーン
class SceneTitle : public SceneBase
{
private:
	static constexpr float PressAnyKeyBlinkingInterval = 0.5f;
	static constexpr int PressAnyKeyHeight = 550;
	static constexpr Vector3 TitleCharacterPos = Vector3(0, -8, 10);
	static constexpr Vector3 TitleLogoPosOffset = Vector3(0, -50, 0);

	static int m_bgmHandle;

	Node* m_rootNode;

	int m_fontHandle;
	bool m_pressAnyKeyFlag;

	float m_elapsedTime;

public:
	SceneTitle() :
		m_rootNode(nullptr),
		m_fontHandle(0),
		m_elapsedTime(0),
		m_pressAnyKeyFlag(false) {}

	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual SceneBase* Update() override;
	virtual void Draw() override;

};