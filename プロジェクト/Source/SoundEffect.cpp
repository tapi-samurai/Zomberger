#include "SoundEffect.h"
#include "DxLib.h"
#include "SoundLoader.h"

// サウンドデータの読み込み
const int SoundEffect::LoadSoundData(const char* soundName, bool create3dFlag, bool duplication)
{
	// 3Dサウンドとして読み込むか設定
	SetCreate3DSoundFlag(create3dFlag);

	// データを読み込んでハンドルを返す
	return SoundLoader::GetInstance()->Load(soundName, duplication);
}

// 2Dサウンドを読み込んで設定
const void SoundEffect::SetSoundEffect2D(int handle, int volume, int pitch, int panPal, int loop)
{
	// ボリューム
	ChangeVolumeSoundMem(volume, handle);

	// ピッチ
	SetFrequencySoundMem(pitch, handle);

	// 左右比率
	ChangePanSoundMem(panPal, handle);
	
	// ループ
	if (loop != -1)
	{
		SetLoopPosSoundMem(loop, handle);
	}
}

// 3Dサウンドを読み込んで設定
const void SoundEffect::SetSoundEffect3D(int handle, const Vector3& position, float radius, int volume, int pitch, int panPal, int loop)
{
	// 共通設定
	SetSoundEffect2D(handle, volume, pitch, panPal, loop);

	// 座標
	Set3DPositionSoundMem(position, handle);

	// 可聴範囲
	Set3DRadiusSoundMem(radius, handle);
}