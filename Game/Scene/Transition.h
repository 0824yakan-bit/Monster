#pragma once
class Transition
{
private:
	struct PositionX
	{
		float Left;
		float Center;
		float Right;
	};
	PositionX m_posX;

	struct PositionY
	{
		float Top;
		float Center;
		float Bottom;
	};
	PositionY m_posY;
	
private://TitletoPlay
	static constexpr float MAX_RADIUS = 2000.0f; // 虹彩の最大半径（1280x720の時）

	float m_irisRadius;     // 虹彩の半径
	int   m_ghIrisScreen;   // 虹彩の描画先（バックバッファ）
private://FieldtoBattle
	float m_battleAngle = 0.0f;
public:
	void Initialize();

	void TitletoPlayOut(float faderate);
	void TitletoPlayIn(float faderate);

	void PlaytoTitleOut(float faderate);
	void PlaytoTitleIn(float faderate);

	void FieldtoBattle_1Out(float faderate);
	void FieldtoBattle_2Out(float faderate);
	void FieldtoBattle_3Out(float faderate);
	void FieldtoBattle_4Out(float faderate);
	void FieldtoBattle_5Out(float faderate);

	void FieldtoBattle_1In (float faderate);
	void FieldtoBattle_2In (float faderate);
	void FieldtoBattle_3In (float faderate);
	void FieldtoBattle_4In (float faderate);
	void FieldtoBattle_5In (float faderate);
	
	void BattletoField_1Out(float faderate);
	void BattletoField_2Out(float faderate);
	
	void BattletoField_1In (float faderate);
	void BattletoField_2In (float faderate);
};

