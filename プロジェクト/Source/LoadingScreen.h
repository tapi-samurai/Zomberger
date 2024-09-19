#pragma once
#include "Sprite.h"

class LoadingScreen
{
private:
	static constexpr Vector2 IconBasePos = Vector2(625, 557);

	Sprite m_sprite;

public:
	LoadingScreen();

	void Release();
	void Draw();
};