#include "SpriteLoader.h"
#include "DxLib.h"

// �I���W�i���̃��f���f�[�^�𐶐����ĕԂ�
int SpriteLoader::LoadResource(const char* dataName)
{
	return LoadGraph(dataName);
}

// �I���W�i���̃��f���f�[�^���폜
void SpriteLoader::DeleteResource(int handle)
{
	DeleteGraph(handle);
}