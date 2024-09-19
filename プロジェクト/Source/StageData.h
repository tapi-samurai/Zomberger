#pragma once
#include <vector>
#include "ItemData.h"

// ステージ情報
namespace StageData
{
	// ステージの情報を持つ構造体
	struct StageConstantValue
	{
	public:
		const char* WallTextureName;
		const char* FloorTextureName;
		const ItemData::RecipeItem FrequentItemName;
		const COLOR_F Color;

		constexpr StageConstantValue(
			const char* wallTextureName,
			const char* floorTextureName,
			ItemData::RecipeItem frequentItemName,
			COLOR_F color
		) :
			WallTextureName(wallTextureName),
			FloorTextureName(floorTextureName),
			FrequentItemName(frequentItemName),
			Color(color) {}
	};

	// ステージの識別用列挙体
	enum class Name
	{
		Field,
		Farm,
		City,
		Alley,
		Soil,

		Length,

		Camp = Length,
	};

	// ステージのデータを持つ配列
	static const std::vector<StageConstantValue> List
	{
		StageConstantValue(
			"Resource/texture/grass01.png",
			"Resource/texture/grass01.png",
			ItemData::RecipeItem::Tomato,
			GetColorF(1.0f, 0, 0, 1.0f)
		),

		StageConstantValue(
			"Resource/texture/stone_floor_d.png",
			"Resource/texture/ground_03.png",
			ItemData::RecipeItem::Lettuce,
			GetColorF(0, 1.0f, 0, 1.0f)
		),

		StageConstantValue(
			"Resource/texture/brick_02.png",
			"Resource/texture/brick_03.png",
			ItemData::RecipeItem::Buns,
			GetColorF(1.0f, 0.4f, 0, 1.0f)
		),

		StageConstantValue(
			"Resource/texture/Wood_Planks_40_Albedo.png",
			"Resource/texture/Demolished_Tiles_2_Albedo.png",
			ItemData::RecipeItem::Cheese,
			GetColorF(1.0f, 1.0f, 0, 1.0f)
		),

		StageConstantValue(
			"Resource/texture/asphalt_01.png",
			"Resource/texture/ground_01.png",
			ItemData::RecipeItem::Patty,
			GetColorF(0.4f, 0, 0, 1.0f)
		),

		StageConstantValue(
			"Resource/texture/brick_04.png",
			"Resource/texture/brick_01.png",
			ItemData::RecipeItem::Null,
			GetColorF(0.87f, 0.42f, 0.22f, 1.0f)
		),
	};
}