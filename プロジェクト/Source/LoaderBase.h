#pragma once
#include <map>
#include <regex>
#include "LoadData.h"

// ���\�[�X��Loader���N���X

class LoaderBase
{
protected:
	std::map<const char*, LoadData> m_dataList;	// ���[�h�����f�[�^�̃��X�g

	virtual int LoadResource(const char* dataName) = 0;
	virtual void DeleteResource(int handle) = 0;

	virtual int GetDuplication(int handle) { return handle; }	// �������K�v�ȏꍇ�ɃI�[�o�[���C�h����֐�

public:
	int Load(const char* dataName, bool duplication = true);
	void Delete(const char* dataName, int duplicationHandle = -1);
};