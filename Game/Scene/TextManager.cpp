#include "pch.h"
#include "Game/Scene/TextManager.h"

#include"Game/InputManager/InputManager.h"
#include"Game/ImageManager/ImageManager.h"
#include"Game/Scene/FieldScene.h"

TextManager::TextManager()
	:m_display{false}
	,m_blinking{false}
	,m_displayTimer{0}
	,m_displayText{}
	,m_receptionTimer{}
	,m_currentTextType{}
	,m_image{}
	,m_gameClear{}
{
	////本文
	m_TextCount[Start]			= 7;
	m_TextCount[Signboard_1]	= 2;
	m_TextCount[Signboard_2]	= 2;
	m_TextCount[Signboard_3]	= 1;
	m_TextCount[Signboard_4]	= 4;
	m_TextCount[End]			= 5;
}

TextManager::~TextManager()
{
}

void TextManager::Initialize()
{
	m_displayTimer = 0;
	m_gameClear = false;
	//本文
	m_TextCount[Start]			= 7;
	m_TextCount[Signboard_1]	= 2;
	m_TextCount[Signboard_2]	= 2;
	m_TextCount[Signboard_3]	= 5;
	m_TextCount[Signboard_4]	= 5;
	m_TextCount[End]			= 5;

	drawTextBoxPosition = {20,500};
	drawTextBoxSize = {1240,180};
}

void TextManager::Update(InputManager& inputManager,FieldScene&fieldScene)
{
	if (!m_display)
	{
		return;
	}
	m_displayTimer++;
	if (m_displayTimer % 20 == 0)
	{
		m_blinking = !m_blinking;
	}
	// Enterでテキストを閉じる
	if (fieldScene.m_count==m_TextCount[m_currentTextType])
	{
		m_display = false;
		fieldScene.m_count = 0;
		switch (m_currentTextType)
		{
		case Start:
			fieldScene.STtext.m_start = false;
			break;
		case Signboard_1:
			fieldScene.STtext.m_signboard_1 = false;
			break;
		case Signboard_2:
			fieldScene.STtext.m_signboard_2 = false;
			break;
		case Signboard_3:
			fieldScene.STtext.m_signboard_3 = false;
		case Signboard_4:
			fieldScene.STtext.m_signboard_4 = false;
			break;
		case End:
			fieldScene.STtext.m_end = false;
			break;
		}
	}
}
void TextManager::Render()
{
	if (!m_display)
	{
		return;
	}

	m_image->DrawCommandbox2(drawTextBoxPosition, drawTextBoxSize);
	SetFontSize(40);
	DrawString(120,550,m_displayText.c_str(), GetColor(255, 255, 255), TRUE);
	SetFontSize(30);
	Vector2 drawNextTextPosition = { 1100,630 };
	Vector2 drawNextTextSize = { 30,30 };
	if(m_blinking)m_image->DrawCommandCursor(drawNextTextPosition, drawNextTextSize);
}
void TextManager::SetImage(ImageManager* image)
{
	m_image = image;
}
void TextManager::SetDisplayText()
{
	m_display = true;
}
bool TextManager::SelectDisplayText()const
{
	return m_display;
}

void TextManager::StartText(int count)
{
	m_display = true;
	m_currentTextType = TextType::Start;
	switch (count)
	{
	case 0:
		m_displayText = L"ようやく起きたか。僕はスライム、よろしく！";
		break;
	case 1:
		m_displayText = L"なんで、閉じ込められているかって？\nあの悪魔のせいさ。";
		break;
	case 2:
		m_displayText = L"奴のせいで、この森は住めない場所になっちまった。";
			break;
	case 3:
		m_displayText = L"だから頼む、あの悪魔を倒して、\n森を平和な場所に戻してくれ！";
			break;
	case 4:
		m_displayText = L"君一人だと心配だから、僕もついていくよ。\n僕は無属性と水属性なら扱えるよ、覚えておいてね。";
			break;
	case 5:
		m_displayText = L"...まずはここから出ないとね。この壁の近くでなら、\nYでメニューを開いて、技の無属性で壊せると思うよ。";
			break;
	case 6:
		m_displayText = L"もし危なくなったら、すぐに個々の場所に戻ってきてね\nここなら安全だからさ。";
		break;
	}
}

void TextManager::SignBoard_1Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_1;
	switch (count)
	{
	case 0:
		m_displayText = L"看板があるね、読んでみるよ\n「落ち葉が邪魔なら風属性の技でどかせます」";
		break;
	case 1:
		m_displayText = L"だって、\nでも僕は風属性の技は持っていないよ？";
		break;
	}
}

void TextManager::SignBoard_2Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_2;
	switch (count)
	{
	case 0:
		m_displayText = L"「水は穴に流し込めます、土は水の上に置けます」\nだって、前は風で落ち葉を飛ばせたし";
		break;
	case 1:
		m_displayText = L"属性ごとにできることが違うのかもね。";
	}
}

void TextManager::SignBoard_3Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_3;
	switch (count)
	{
	case 0:
		m_displayText = L"「複数の技を組み合わせることで\n強力な攻撃ができます";
		break;
	}
}

void TextManager::SignBoard_4Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_4;
	switch (count)
	{
	case 0:
		m_displayText = L"この看板は、あの悪魔がかいたみたいだよ\n一応読んでみようか";
		break;
	case 1:
		m_displayText = L"「我に謁見を望む……ならば、\n岩と鳥と竜を打ち倒してみせよ";
		break;
	case 2:
		m_displayText=L"見事、すべてを屠った暁には\n貴様に我が魔城へ立ち入る権利を授けてやろう」";
		break;
	case 3:
		m_displayText = L"岩？鳥？竜？わからないけど、とにかく探すしかないね。";
		break;
	}
}

void TextManager::EndText(int count)
{
	m_display = true;

	m_currentTextType = TextType::End;
	switch (count)
	{
	case 0:
		m_displayText = L"...酷い有様になっちゃったね。\nでも、もうあの悪魔はいない。";
		break;
	case 1:
		m_displayText = L"じゃあ次は僕たちの番だ。壊れてしまったものは多い\nけれど、また住みやすい場所に戻していくよ。";
		break;
	case 2:
		m_displayText = L"きっと、前よりももっといい場所にできる。\nここまで一緒に戦ってきてくれて、本当にありがとう。";
		break;
	case 3:
		m_displayText = L"またいつか、遊びに来てよ。じゃあね";
		break;
	case 4:
		m_gameClear = true;
		break;
	}
}

bool TextManager::GameClear()
{
	return m_gameClear;
}