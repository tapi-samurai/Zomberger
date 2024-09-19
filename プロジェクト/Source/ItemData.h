#pragma once
#include <vector>

// �A�C�e�����
namespace ItemData
{
	// �A�C�e���̏������\����
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

	// �f�ރA�C�e���̎��ʗp�񋓑�
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

	// �A�C�e���̃f�[�^�����z��
	static const std::vector<ItemConstantValue> List
	{
		{ ItemConstantValue("Null", nullptr)},
		{ ItemConstantValue("�o���Y", "Resource/texture/buns_shadow.png")},
		{ ItemConstantValue("�p�e�B", "Resource/texture/patty_shadow.png")},
		{ ItemConstantValue("�`�[�Y", "Resource/texture/cheese_shadow.png")},
		{ ItemConstantValue("�g�}�g", "Resource/texture/tomato_shadow.png")},
		{ ItemConstantValue("���^�X", "Resource/texture/lettuce_shadow.png")},
		{ ItemConstantValue("Hamburger", "Resource/texture/humburger.png", 2, 2)},
		{ ItemConstantValue("Scoop", "Resource/texture/scoop.png")},
	};
}