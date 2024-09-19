#include "SpriteLoader.h"
#include "DxLib.h"

// オリジナルのモデルデータを生成して返す
int SpriteLoader::LoadResource(const char* dataName)
{
	return LoadGraph(dataName);
}

// オリジナルのモデルデータを削除
void SpriteLoader::DeleteResource(int handle)
{
	DeleteGraph(handle);
}