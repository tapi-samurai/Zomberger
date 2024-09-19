#include "ModelLoader.h"
#include "DxLib.h"

// オリジナルのモデルデータを生成して返す
int ModelLoader::LoadResource(const char* dataName)
{
	return MV1LoadModel(dataName);
}

// オリジナルのモデルデータを削除
void ModelLoader::DeleteResource(int handle)
{
	MV1DeleteModel(handle);
}

// オリジナルのモデルハンドルから複製を作って返す
int ModelLoader::GetDuplication(int handle)
{
	return MV1DuplicateModel(handle);
}