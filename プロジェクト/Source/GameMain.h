#pragma once

class SceneManager;

class GameMain
{
private:
	SceneManager* m_sceneManager;
	int m_screen;

public:
	GameMain();
	~GameMain();

	void Run();
};
