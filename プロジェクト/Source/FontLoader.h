#pragma once
#include "LoaderBase.h"

class FontLoader : public LoaderBase
{
private:
	int LoadResource(const char* dataName) override;
	void DeleteResource(int handle) override;

public:
	// ƒVƒ“ƒOƒ‹ƒgƒ“
	static FontLoader* GetInstance()
	{
		static FontLoader instance;
		return &instance;
	}
};