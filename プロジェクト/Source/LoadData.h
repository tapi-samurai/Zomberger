#pragma once

// �摜�f�[�^�Z�b�g
class LoadData
{
public:
	const int handle;			// �摜�̃O���t�B�b�N�n���h��
	int count;					// �摜���g�p����I�u�W�F�N�g�̐�

	LoadData(int handle) :
		handle(handle),
		count(0) {}
};