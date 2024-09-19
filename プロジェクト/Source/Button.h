#pragma once
#include "Vector2.h"
#include <functional>

class Button
{
private:
	Vector2 m_size;
	int m_mouseButton;
	std::function<void()> m_callback;
	bool m_isClickDown;

	bool IsMouseContain(const Vector2& position) const;

public:
	Button(const Vector2& size, int mouseButton, const std::function<void()>& callback) :
		m_size(size),
		m_mouseButton(mouseButton),
		m_callback(callback),
		m_isClickDown(false) {}

	void Update(const Vector2& position);
	void Draw(const Vector2& position);
};