#pragma once
#include"Game/Scene/Transition.h"
class TransitionManager
{	
public:
	//トランジション種類
	enum class FadeType
	{
		TitletoPlayOut,
		TitletoPlayIn,

		PlaytoTitleOut,////
		PlaytoTitleIn,////

		FieldtoBattle_1Out,//level0の時
		FieldtoBattle_2Out,//level1の時
		FieldtoBattle_3Out,//level2の時
		FieldtoBattle_4Out,//level3の時
		FieldtoBattle_5Out,//level4の時

		FieldtoBattle_1In,//level0の時
		FieldtoBattle_2In,//level1の時
		FieldtoBattle_3In,//level2の時
		FieldtoBattle_4In,//level3の時
		FieldtoBattle_5In,//level4の時

		BattletoField_1Out,//勝利
		BattletoField_2Out,//逃げる

		BattletoField_1In,//勝利
		BattletoField_2In,//逃げる

		None,
	};
	FadeType m_fadeType;
private:
	Transition m_transition;

	//トランジション状態
	enum class TransitionState
	{
		None,
		FadeOut,
		Max,
		FadeIn
	};
	TransitionState m_transitionState;



	static constexpr int FADE_FRAME = 60;//２秒
	int m_frameConunt;

public:
	TransitionManager();
	~TransitionManager();

	void Initialize();
	void Update();
	void Render();

	void StartFadeOut();
	void StartFadeIn();

	TransitionManager::TransitionState GetState();
	bool IsMax();
	bool IsNone();
	void SetFadeType(FadeType type);
};