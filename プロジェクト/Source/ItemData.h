#pragma once
#include <vector>

// アイテム情報
namespace ItemData
{
	// アイテムの情報を持つ構造体
	struct ItemConstantValue
	{
	public:
		const char* ItemName;
		const char* TextureName;
		int HealthRecoveryValue;
		int SatietyRecoveryValue;

		constexpr ItemConstantValue(const char* itemName, const char* textureName, int healthRecoveryValue = 0, int satietyRecoveryValue = 0) :
			ItemName(itemName),
			TextureName(textureName),
			HealthRecoveryValue(healthRecoveryValue),
			SatietyRecoveryValue(satietyRecoveryValue) {}
	};

	// 素材アイテムの識別用列挙体
	enum class RecipeItem
	{
		Null = 0,

		Buns,
		Patty,
		Cheese,
		Tomato,
		Lettuce,

		Length,
	};

	enum class OtherItem
	{
		Hamburger = RecipeItem::Length,
		Scoop,

		AllLength,
	};

	// アイテムのデータを持つ配列
	static const std::vector<ItemConstantValue> List
	{
		{ ItemConstantValue("Null", nullptr)},
		{ ItemConstantValue("バンズ", "Resource/texture/buns_shadow.png")},
		{ ItemConstantValue("パティ", "Resource/texture/patty_shadow.png")},
		{ ItemConstantValue("チーズ", "Resource/texture/cheese_shadow.png")},
		{ ItemConstantValue("トマト", "Resource/texture/tomato_shadow.png")},
		{ ItemConstantValue("レタス", "Resource/texture/lettuce_shadow.png")},
		{ ItemConstantValue("Hamburger", "Resource/texture/humburger.png", 2, 2)},
		{ ItemConstantValue("Scoop", "Resource/texture/scoop.png")},
	};
}