#pragma once
#include <vector>

// �e�N�X�`���̒萔���X�g
namespace TextureData
{
	// �e�N�X�`���̒萔�f�[�^�����\����
	struct TextureConstantValue
	{
	public:
		const char* TextureName;

		constexpr TextureConstantValue(const char* textureName) :
			TextureName(textureName) {}
	};

	// �e�N�X�`���̎��ʗp�񋓑�
	enum class Name
	{
		ItemSlot,
		SelectSlot,
		Calendar,
		KeyNeutralF,
		KeyPressF,
		KeyBackGround,
		LoadingIcon,
		CookButton,
		TradeButton,
	};

	// �e�N�X�`���̃f�[�^�����z��
	static const std::vector<TextureConstantValue> List =
	{
		{ TextureConstantValue("Resource/texture/debug_slot.png") },
		{ TextureConstantValue("Resource/texture/debug_arrow.png") },
		{ TextureConstantValue("Resource/texture/calendar.png") },
		{ TextureConstantValue("Resource/texture/keyboard_f.png") },
		{ TextureConstantValue("Resource/texture/keyboard_f_outline.png") },
		{ TextureConstantValue("Resource/texture/key_background.png") },
		{ TextureConstantValue("Resource/texture/loading_icon.png") },
		{ TextureConstantValue("Resource/texture/cookButton.png") },
		{ TextureConstantValue("Resource/texture/tradeButton.png") },
	};
}