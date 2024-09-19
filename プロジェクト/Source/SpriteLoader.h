#pragma once
#include "LoaderBase.h"

class SpriteLoader : public LoaderBase
{
private:
	int LoadResource(const char* dataName) override;
	void DeleteResource(int handle) override;

public:
	// ƒVƒ“ƒOƒ‹ƒgƒ“
	static SpriteLoader* GetInstance()
	{
		static SpriteLoader instance;
		return &instance;
	}
};