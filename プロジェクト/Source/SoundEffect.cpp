#include "SoundEffect.h"
#include "DxLib.h"
#include "SoundLoader.h"

// �T�E���h�f�[�^�̓ǂݍ���
const int SoundEffect::LoadSoundData(const char* soundName, bool create3dFlag, bool duplication)
{
	// 3D�T�E���h�Ƃ��ēǂݍ��ނ��ݒ�
	SetCreate3DSoundFlag(create3dFlag);

	// �f�[�^��ǂݍ���Ńn���h����Ԃ�
	return SoundLoader::GetInstance()->Load(soundName, duplication);
}

// 2D�T�E���h��ǂݍ���Őݒ�
const void SoundEffect::SetSoundEffect2D(int handle, int volume, int pitch, int panPal, int loop)
{
	// �{�����[��
	ChangeVolumeSoundMem(volume, handle);

	// �s�b�`
	SetFrequencySoundMem(pitch, handle);

	// ���E�䗦
	ChangePanSoundMem(panPal, handle);
	
	// ���[�v
	if (loop != -1)
	{
		SetLoopPosSoundMem(loop, handle);
	}
}

// 3D�T�E���h��ǂݍ���Őݒ�
const void SoundEffect::SetSoundEffect3D(int handle, const Vector3& position, float radius, int volume, int pitch, int panPal, int loop)
{
	// ���ʐݒ�
	SetSoundEffect2D(handle, volume, pitch, panPal, loop);

	// ���W
	Set3DPositionSoundMem(position, handle);

	// ���͈�
	Set3DRadiusSoundMem(radius, handle);
}