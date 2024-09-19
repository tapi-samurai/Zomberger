#pragma once

// 画像データセット
class LoadData
{
public:
	const int handle;			// 画像のグラフィックハンドル
	int count;					// 画像を使用するオブジェクトの数

	LoadData(int handle) :
		handle(handle),
		count(0) {}
};