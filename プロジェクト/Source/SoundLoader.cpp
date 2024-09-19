#include "SoundLoader.h"
#include "DxLib.h"

// オリジナルのモデルデータを生成して返す
int SoundLoader::LoadResource(const char* dataName)
{
	return LoadSoundMem(dataName);
}

// オリジナルのモデルデータを削除
void SoundLoader::DeleteResource(int handle)
{
	DeleteSoundMem(handle);
}

// オリジナルのモデルハンドルから複製を作って返す
int SoundLoader::GetDuplication(int handle)
{
	return DuplicateSoundMem(handle);
}