#pragma once
#include <map>
#include <regex>
#include "LoadData.h"

// リソースのLoader基底クラス

class LoaderBase
{
protected:
	std::map<const char*, LoadData> m_dataList;	// ロードしたデータのリスト

	virtual int LoadResource(const char* dataName) = 0;
	virtual void DeleteResource(int handle) = 0;

	virtual int GetDuplication(int handle) { return handle; }	// 複製が必要な場合にオーバーライドする関数

public:
	int Load(const char* dataName, bool duplication = true);
	void Delete(const char* dataName, int duplicationHandle = -1);
};