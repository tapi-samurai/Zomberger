#pragma once
#include "SceneBase.h"

class Node;

class SceneResult : public SceneBase
{
private:
	static constexpr int TextHeight = 80;
	static constexpr int TextWidth = 250;

	Node* m_rootNode;

	int m_fontHandle;

public:
	SceneResult() :
		m_rootNode(nullptr),
		m_fontHandle(0) {}

	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual SceneBase* Update() override;
	virtual void Draw() override;
};