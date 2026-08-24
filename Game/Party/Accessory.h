#pragma once
#include<array>
class Accessory
{
public:
	enum ElementType
	{
		NOMAL,
		FIRE,
		WATER,
		GRASS,
		SOIL,
		THUNDER,
		WIND,
		NONE_MAX,
	};

	struct UpgradeAccessory
	{
		ElementType type;

		int level;

		int damage;

		int top;
		int under;
		int left;
		int right;
	};

	UpgradeAccessory m_accessory[static_cast<int>(ElementType::NONE_MAX)];
	std::array<ElementType,NONE_MAX>elementTypes=
	{
		NOMAL,
		FIRE,
		WATER,
		GRASS,
		SOIL,
		THUNDER,
		WIND
	};
public:

	Accessory();
	~Accessory();
	void Initialize();

	void Upgrade(ElementType type);

	Accessory::UpgradeAccessory GetAccessory(ElementType type)const;
	const wchar_t* GetElementName(ElementType type);
};

