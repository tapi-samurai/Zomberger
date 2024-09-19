#pragma once
#include <vector>

// テクスチャの定数リスト
namespace TextureData
{
	// テクスチャの定数データを持つ構造体
	struct TextureConstantValue
	{
	public:
		const char* TextureName;

		constexpr TextureConstantValue(const char* textureName) :
			TextureName(textureName) {}
	};

	// テクスチャの識別用列挙体
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

	// テクスチャのデータを持つ配列
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