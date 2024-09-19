#pragma once
#include <vector>

// �t�H���g���
namespace FontData
{
	// �t�H���g���ʗp�񋓑�
	enum class Name
	{
		CinecaptionNum,
		CinecaptionAll,
		CinecaptionText,
	};

	// �t�H���g�̃f�[�^�����z��
	static const std::vector<const char*> List
	{
		{"Resource/font/cinecaption_num.dft"},
		{"Resource/font/cinecaption_all.dft"},
		{"Resource/font/cinecaption_text.dft"},
	};
}