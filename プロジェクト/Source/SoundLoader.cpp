#include "SoundLoader.h"
#include "DxLib.h"

// �I���W�i���̃��f���f�[�^�𐶐����ĕԂ�
int SoundLoader::LoadResource(const char* dataName)
{
	return LoadSoundMem(dataName);
}

// �I���W�i���̃��f���f�[�^���폜
void SoundLoader::DeleteResource(int handle)
{
	DeleteSoundMem(handle);
}

// �I���W�i���̃��f���n���h�����畡��������ĕԂ�
int SoundLoader::GetDuplication(int handle)
{
	return DuplicateSoundMem(handle);
}