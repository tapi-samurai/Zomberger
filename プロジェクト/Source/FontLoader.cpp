#include "FontLoader.h"
#include "DxLib.h"

// オリジナルのモデルデータを生成して返す
int FontLoader::LoadResource(const char* dataName)
{
	return LoadFontDataToHandle(dataName);
}

// オリジナルのモデルデータを削除
void FontLoader::DeleteResource(int handle)
{
	DeleteFontToHandle(handle);
}