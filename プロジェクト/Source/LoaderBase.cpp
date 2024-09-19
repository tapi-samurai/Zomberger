#include "LoaderBase.h"
#include "LoadData.h"

int LoaderBase::Load(const char* dataName, bool duplication)
{
	LoadData* loadData = nullptr;

	// �摜�f�[�^�����X�g������o��
	auto data = m_dataList.find(dataName);
	if (data != m_dataList.end())
	{
		loadData = &data->second;
	}
	else
	{
		// �摜�f�[�^�����X�g�ɂȂ���ΐ������ēo�^

		// ���[�h
		int handle = LoadResource(dataName);

		// ���X�g�ɓo�^
		auto result = m_dataList.emplace(dataName, LoadData(handle));
		loadData = &result.first->second;
	}

	// �I�u�W�F�N�g�̐����J�E���g
	loadData->count++;

	// �f�[�^�̕������K�v�Ȃ畡������
	int handle = loadData->handle;

	if (duplication == true)
	{
		handle = GetDuplication(handle);
	}

	// ���������n���h����Ԃ�
	return handle;
}

void LoaderBase::Delete(const char* dataName, int duplicationHandle)
{
	LoadData* loadData = nullptr;

	// �摜�f�[�^�����X�g������o��
	auto data = m_dataList.find(dataName);
	if (data != m_dataList.end())
	{
		loadData = &data->second;
	}

	// ���X�g�ɓo�^����Ă��Ȃ������牽�����Ȃ�
	if (!loadData) return;

	// �J�E���g�����炷
	loadData->count--;

	// �J�E���g���O��������摜���������������
	if (!loadData->count)
	{
		DeleteResource(loadData->handle);

		// ���X�g������폜����
		m_dataList.erase(dataName);
	}

	// �������ꂽ�n���h���ł���΃������������
	if (duplicationHandle != -1)
	{
		DeleteResource(duplicationHandle);
	}
}