#pragma once
#include "Vector3.h"

class SoundEffect
{
public:
	// シングルトン
	static SoundEffect* GetInstance()
	{
		static SoundEffect instance;
		return &instance;
	}

	// サウンドデータの読み込み
	const int LoadSoundData(const char* soundName, bool create3dFlag = false, bool duplication = true);

	// 2Dサウンドを読み込んで設定
	const void SetSoundEffect2D(int handle, int volume, int pitch = -1, int panPal = 0, int loop = -1);

	// 3Dサウンドを読み込んで設定
	const void SetSoundEffect3D(int handle, const Vector3& position, float radius, int volume, int pitch = -1, int panPal = 0, int loop = -1);
};