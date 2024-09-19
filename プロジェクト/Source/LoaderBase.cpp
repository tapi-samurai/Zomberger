#include "LoaderBase.h"
#include "LoadData.h"

int LoaderBase::Load(const char* dataName, bool duplication)
{
	LoadData* loadData = nullptr;

	// 画像データをリストから取り出す
	auto data = m_dataList.find(dataName);
	if (data != m_dataList.end())
	{
		loadData = &data->second;
	}
	else
	{
		// 画像データがリストになければ生成して登録

		// ロード
		int handle = LoadResource(dataName);

		// リストに登録
		auto result = m_dataList.emplace(dataName, LoadData(handle));
		loadData = &result.first->second;
	}

	// オブジェクトの数をカウント
	loadData->count++;

	// データの複製が必要なら複製する
	int handle = loadData->handle;

	if (duplication == true)
	{
		handle = GetDuplication(handle);
	}

	// 生成したハンドルを返す
	return handle;
}

void LoaderBase::Delete(const char* dataName, int duplicationHandle)
{
	LoadData* loadData = nullptr;

	// 画像データをリストから取り出す
	auto data = m_dataList.find(dataName);
	if (data != m_dataList.end())
	{
		loadData = &data->second;
	}

	// リストに登録されていなかったら何もしない
	if (!loadData) return;

	// カウントを減らす
	loadData->count--;

	// カウントが０だったら画像をメモリから消去
	if (!loadData->count)
	{
		DeleteResource(loadData->handle);

		// リストからも削除する
		m_dataList.erase(dataName);
	}

	// 複製されたハンドルであればメモリから消去
	if (duplicationHandle != -1)
	{
		DeleteResource(duplicationHandle);
	}
}