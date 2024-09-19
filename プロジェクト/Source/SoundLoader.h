#pragma once
#include "LoaderBase.h"

class SoundLoader : public LoaderBase
{
private:
	int LoadResource(const char* dataName) override;
	void DeleteResource(int handle)	override;

	int GetDuplication(int handle) override;

public:
	// ƒVƒ“ƒOƒ‹ƒgƒ“
	static SoundLoader* GetInstance()
	{
		static SoundLoader instance;
		return &instance;
	}
};