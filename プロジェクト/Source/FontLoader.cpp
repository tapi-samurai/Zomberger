#include "FontLoader.h"
#include "DxLib.h"

// �I���W�i���̃��f���f�[�^�𐶐����ĕԂ�
int FontLoader::LoadResource(const char* dataName)
{
	return LoadFontDataToHandle(dataName);
}

// �I���W�i���̃��f���f�[�^���폜
void FontLoader::DeleteResource(int handle)
{
	DeleteFontToHandle(handle);
}