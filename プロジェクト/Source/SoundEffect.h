#pragma once
#include "Vector3.h"

class SoundEffect
{
public:
	// �V���O���g��
	static SoundEffect* GetInstance()
	{
		static SoundEffect instance;
		return &instance;
	}

	// �T�E���h�f�[�^�̓ǂݍ���
	const int LoadSoundData(const char* soundName, bool create3dFlag = false, bool duplication = true);

	// 2D�T�E���h��ǂݍ���Őݒ�
	const void SetSoundEffect2D(int handle, int volume, int pitch = -1, int panPal = 0, int loop = -1);

	// 3D�T�E���h��ǂݍ���Őݒ�
	const void SetSoundEffect3D(int handle, const Vector3& position, float radius, int volume, int pitch = -1, int panPal = 0, int loop = -1);
};