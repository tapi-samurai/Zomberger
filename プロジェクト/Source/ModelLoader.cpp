#include "ModelLoader.h"
#include "DxLib.h"

// �I���W�i���̃��f���f�[�^�𐶐����ĕԂ�
int ModelLoader::LoadResource(const char* dataName)
{
	return MV1LoadModel(dataName);
}

// �I���W�i���̃��f���f�[�^���폜
void ModelLoader::DeleteResource(int handle)
{
	MV1DeleteModel(handle);
}

// �I���W�i���̃��f���n���h�����畡��������ĕԂ�
int ModelLoader::GetDuplication(int handle)
{
	return MV1DuplicateModel(handle);
}