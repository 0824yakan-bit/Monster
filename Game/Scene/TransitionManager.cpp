#include "pch.h"
#include "Game/Scene/TransitionManager.h"



TransitionManager::TransitionManager()
{
}

TransitionManager::~TransitionManager()
{
}

void TransitionManager::Initialize()
{
	m_transition.Initialize();
	m_transitionState = TransitionState::None;
	m_fadeType = FadeType::None;
}

void TransitionManager::Update()
{
	switch (m_transitionState)
	{
	case TransitionState::None:
		break;

	case TransitionState::FadeOut:
		m_frameConunt++;
			if (m_frameConunt >= FADE_FRAME)
			{
				m_frameConunt = FADE_FRAME;
				m_transitionState = TransitionState::Max;
			}
		break;

	case TransitionState::Max:

		break;

	case TransitionState::FadeIn:
		m_frameConunt-=2;
		if (m_frameConunt <= 0)
		{
			m_frameConunt = 0;
			m_transitionState = TransitionState::None;
			m_transition.Initialize();
		}
		break;
	}
}

void TransitionManager::Render()
{
	if (m_transitionState == TransitionState::None)return;

	const float fadeRate = static_cast<float>(m_frameConunt) / FADE_FRAME;

	switch (m_fadeType)
	{
	case FadeType::TitletoPlayOut:
		m_transition.TitletoPlayOut(fadeRate);
		break;
	
	case FadeType::PlaytoTitleOut:
		m_transition.PlaytoTitleOut(fadeRate);
		break;

	case FadeType::FieldtoBattle_1Out:
		m_transition.FieldtoBattle_1Out(fadeRate);
		break;

	case FadeType::FieldtoBattle_2Out:
		m_transition.FieldtoBattle_2Out(fadeRate);
		break;

	case FadeType::FieldtoBattle_3Out:
		m_transition.FieldtoBattle_3Out(fadeRate);
		break;

	case FadeType::FieldtoBattle_4Out:
		m_transition.FieldtoBattle_4Out(fadeRate);
		break;

	case FadeType::FieldtoBattle_5Out:
		m_transition.FieldtoBattle_5Out(fadeRate);
		break;

	case FadeType::BattletoField_1Out:
		m_transition.BattletoField_1Out(fadeRate);
		break;

	case FadeType::BattletoField_2Out:
		m_transition.BattletoField_2Out(fadeRate);
		break;

	case FadeType::TitletoPlayIn:
		m_transition.TitletoPlayIn(fadeRate);
		break;

	case FadeType::PlaytoTitleIn:
		m_transition.PlaytoTitleIn(fadeRate);
		break;

	case FadeType::FieldtoBattle_1In:
		m_transition.FieldtoBattle_1In(fadeRate);
		break;

	case FadeType::FieldtoBattle_2In:
		m_transition.FieldtoBattle_2In(fadeRate);
		break;

	case FadeType::FieldtoBattle_3In:
		m_transition.FieldtoBattle_3In(fadeRate);
		break;

	case FadeType::FieldtoBattle_4In:
		m_transition.FieldtoBattle_4In(fadeRate);
		break;

	case FadeType::FieldtoBattle_5In:
		m_transition.FieldtoBattle_5In(fadeRate);
		break;

	case FadeType::BattletoField_1In:
		m_transition.BattletoField_1In(fadeRate);
		break;

	case FadeType::BattletoField_2In:
		m_transition.BattletoField_2In(fadeRate);
		break;

	case FadeType::None:
		break;
	}

}

void TransitionManager::StartFadeOut()
{
	m_transitionState = TransitionState::FadeOut;
	m_frameConunt = 0;
}

void TransitionManager::StartFadeIn()
{
	m_transitionState = TransitionState::FadeIn;
	m_frameConunt = FADE_FRAME;
}

TransitionManager::TransitionState TransitionManager::GetState()
{
	return m_transitionState;
}

bool TransitionManager::IsMax()
{
	return m_transitionState==TransitionState::Max;
}

bool TransitionManager::IsNone()
{
	return m_transitionState == TransitionState::None;
}

void TransitionManager::SetFadeType(FadeType type)
{
	m_fadeType = type;
}
