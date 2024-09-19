#pragma once
#include <vector>

// フォント情報
namespace FontData
{
	// フォント識別用列挙体
	enum class Name
	{
		CinecaptionNum,
		CinecaptionAll,
		CinecaptionText,
	};

	// フォントのデータを持つ配列
	static const std::vector<const char*> List
	{
		{"Resource/font/cinecaption_num.dft"},
		{"Resource/font/cinecaption_all.dft"},
		{"Resource/font/cinecaption_text.dft"},
	};
}