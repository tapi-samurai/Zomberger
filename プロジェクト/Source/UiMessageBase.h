#pragma once
#include "Node.h"
#include "Vector2.h"

class UiMessageBase : public Node
{
protected:
	bool m_isDisp;

public:
	UiMessageBase() :
		m_isDisp(false) {}

	void OnDisplay() { m_isDisp = true; }
};