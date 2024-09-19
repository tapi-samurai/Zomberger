#include "DxLib.h"
#include "GameMain.h"

#ifdef _DEBUG
#include <crtdbg.h>	// _CrtSetDbgFlag
#endif

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// メモリリーク検出
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// 起動
	GameMain game;
	game.Run();

	return 0;
}