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
	,m_isTyping{}
	,m_fpsCounter{}
	, m_displaytextLength{}
	,m_displaySpeed{}
{
	////本文
	m_TextCount[Start]			= 7;
	m_TextCount[Signboard_1]	= 2;
	m_TextCount[Signboard_2]	= 2;
	m_TextCount[Signboard_3]	= 1;
	m_TextCount[Signboard_4]	= 4;
	m_TextCount[Boss_1] = 2;
	m_TextCount[Boss_2] = 3;
	m_TextCount[Boss_3] = 3;
	m_TextCount[LastBossAlive] = 14;
	m_TextCount[LastBossDefeated]= 9;
	m_TextCount[End]			= 5;
}

TextManager::~TextManager()
{
}

void TextManager::Initialize()
{
	m_displayTimer = 0;
	m_fpsCounter = 0;
	m_displaytextLength = 0;
	m_displaySpeed = 3;//テキスト表示スピード小さいほど速い
	m_isTyping = true;
	m_gameClear = false;
	m_gameOver = false;
	//本文
	m_TextCount[Start]			= 7;
	m_TextCount[Signboard_1]	= 2;
	m_TextCount[Signboard_2]	= 2;
	m_TextCount[Signboard_3]	= 1;
	m_TextCount[Signboard_4]	= 4;
	m_TextCount[Boss_1] = 2;
	m_TextCount[Boss_2] = 3;
	m_TextCount[Boss_3] = 3;
	m_TextCount[LastBossAlive] = 14;
	m_TextCount[LastBossDefeated]= 9;
	m_TextCount[End]			= 5;

	drawTextBoxPosition = {20,500};
	drawTextBoxSize = {1240,180};
}

void TextManager::Update(InputManager& inputManager, FieldScene& fieldScene)
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

	TypeWriter(inputManager);
	// Enterでテキストを閉じる
	if (fieldScene.m_count == m_TextCount[m_currentTextType])
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
			break;
		case Signboard_4:
			fieldScene.STtext.m_signboard_4 = false;
			break;
		case Boss_1:
			fieldScene.STtext.m_boss_1 = false;
			break;
		case Boss_2:
			fieldScene.STtext.m_boss_2 = false;
			break;
		case Boss_3:
			fieldScene.STtext.m_boss_3 = false;
			break;
		case LastBossAlive:
			fieldScene.STtext.m_lastBossAlive = false;
			break;
		case LastBossDefeated:
			fieldScene.STtext.m_lastBossDefeated = false;
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
	Vector2 slimeTextPosition = { 20,430 };
	Vector2 slimeTextSize = { 320,75 };
	m_image->DrawCommandbox1(slimeTextPosition, slimeTextSize);
	m_image->DrawCommandbox2(drawTextBoxPosition, drawTextBoxSize);
	SetFontSize(40);
	DrawString(80, 446, m_currentChara.c_str(), GetColor(255, 255, 255), TRUE);
	DrawString(120,550,m_currentdisplayText.c_str(), GetColor(255, 255, 255), TRUE);
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
	m_currentChara = L"スライム";
	switch (count)
	{
	case 0:
		m_displayText = L"ようやく起きたね。僕はスライム、よろしく！";
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
		m_displayText = L"うーん...君一人だと心配だから、僕もついていくよ。\n僕は無属性と水属性なら教えられるよ、覚えておいてね。";
			break;
	case 5:
		m_displayText = L"...まずはここから出ないとね。この壁の近くでなら、\nYでメニューを開いて、技の無属性で壊せると思うよ。";
			break;
	case 6:
		m_displayText = L"もし危なくなったら、すぐにここ場所に戻ってきてね\nここなら安全だからさ。";
		break;
	}
}

void TextManager::SignBoard_1Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_1;
	m_currentChara = L"スライム";
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
	m_currentChara = L"スライム";
	switch (count)
	{
	case 0:
		m_displayText = L"「水は穴に流し込めます、土は水の上に置けます」\nだって、前は風で落ち葉を飛ばせたし";
		break;
	case 1:
		m_displayText = L"属性ごとにできることが違うのかもね。";
	}
}

void TextManager::SignBoard_3Text(int count)////
{
	m_display = true;

	m_currentTextType = TextType::Signboard_3;
	m_currentChara = L"スライム";
	switch (count)
	{
	case 0:
		m_displayText = L"「複数の技を組み合わせることで\n強力な攻撃ができます」";
		break;
	}
}

void TextManager::SignBoard_4Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_4;
	m_currentChara = L"スライム";
	switch (count)
	{
	case 0:
		m_displayText = L"この看板は、あの悪魔がかいたみたいだよ\n一応読んでみようか";
		break;
	case 1:
		m_displayText = L"「我に謁見を望むならば、\n岩と鳥と竜を打ち倒してみせよ";
		break;
	case 2:
		m_displayText=L"見事、すべてを屠った暁には\n貴様に我が魔城へ立ち入る権利を授けてやろう」";
		break;
	case 3:
		m_displayText = L"岩？鳥？竜？わからないけど、とにかく探すしかないね。";
		break;
	}
}

void TextManager::Boss_1Text(int count)////
{
	m_display = true;

	m_currentTextType = TextType::Boss_1;
	m_currentChara = L"？？？？？";
	switch (count)
	{
	case 0:
		m_displayText = L"……岩の魔獣が……倒されたか。\nまさか、ここまで辿り着くとはな……。";
		break;
	case 1:
		m_displayText = L"……この程度では、まだ……。\nこの先へ進めば、もう後戻りはできぬぞ。";
		break;
	}
}

void TextManager::Boss_2Text(int count)////
{
	m_display = true;

	m_currentTextType = TextType::Boss_2;
	m_currentChara = L"？？？？？";
	switch (count)
	{
	case 0:
		m_displayText = L"……鳥の魔獣まで倒したか。\nなぜ、そこまでして進む……。";
		break;
	case 1:
		m_displayText = L"「お前たちは、何も知らない\nこの世界で起きていることも……。";
		break;
	case 2:
		m_displayText = L"「この先に進めば、いずれ分かる。\nだが……これ以上、邪魔をされるわけにはいかない";
		break;
	}
}

void TextManager::Boss_3Text(int count)////
{
	m_display = true;

	m_currentTextType = TextType::Boss_3;
	m_currentChara = L"？？？？？";
	switch (count)
	{
	case 0:
		m_displayText = L"「……竜まで……。\n……ここまで来たのなら、仕方がない。";
		break;
	case 1:
		m_displayText = L"「お前たちは……私を悪だと思っているのだろう。\nだが……見えているものだけが、すべてだとは限らない。";
		break;
	case 2:
		m_displayText = L"「この先で、確かめるがいい。\nお前たちが信じてきたものが、本当に正しいのかを……。";
		break;
	}
}

void TextManager::LastBossAliveText(int count)
{
	m_display = true;

	m_currentTextType = TextType::LastBossAlive;
	switch (count)
	{
	case 0:
		m_currentChara = L"ディアボロ";
		m_displayText = L"……そうか……私は……負けたのか……\nだが……なぜだ……？なぜ、お前たちは……。";
		break;
	case 1:
		m_displayText = L"我はただ……この森を修復していたに過ぎない……\n荒れ果てた大地を戻し、緑で満たそうとしていただけだ……";
		break;
	case 2:
		m_currentChara = L"スライム";
		m_displayText = L"……え？じゃあ……ディアボロは、\n本当はこの森を壊していなかったの？";
		break;
	case 3:
		m_currentChara = L"ディアボロ";
		m_displayText = L"そうだ。\nお前たちがずっと見ていたこの壊れた大地も――";
		break;
	case 4:
		m_displayText = L"我が力を使い、修復している途中だったのだ。\n……だが、我の力だけではもう限界だ。";
		break;
	case 5:
		m_currentChara = L"スライム";
		m_displayText = L"だったら……僕たちも手伝うよ。ここまで一緒に来たんだ。\n今度は壊れた場所を、みんなで直そう！";
		break;
	case 6:
		m_currentChara = L"ディアボロ";
		m_displayText = L"……お前たちが……？\n……ならば、一つ頼みがある。";
		break;
	case 7:
		m_displayText = L"我は、この土地を緑で満たす。\nお前たちは……新しい道を作れ。";
		break;
	case 8:
		m_currentChara = L"スライム";
		m_displayText = L"もちろん！\n壊れたところは直して、";
		break;
	case 9:
		m_displayText = L"通れないところには新しい道を作る。\nそうすればきっと……前よりもっと、いい場所にできるよ！";
		break;
	case 10:
		m_currentChara = L"ディアボロ";
		m_displayText = L"……そうだな。\nこの世界は、一度壊れたくらいでは終わらない。";
		break;
	case 11:
		m_currentChara = L"スライム";
		m_displayText = L"じゃあ……これからも一緒だね！";
		break;
	case 12:
		m_currentChara = L"ディアボロ";
		m_displayText = L"……ああ。\n今度は……共に、この世界を作っていこう。";
		break;
	case 13:
		m_gameClear = true;
		break;
	}
}
void TextManager::LastBossDefeatedText(int count)////
{
	m_display = true;

	m_currentTextType = TextType::LastBossDefeated;
	switch (count)
	{
	case 0:
		m_currentChara = L"ディアボロ";
		m_displayText = L"…そうか、私は負けたのか…\nだが…なぜそこまでして…";
		break;
	case 1:
		m_displayText = L"……私は、この世界を壊していたのではない。\n我はただ、この森を修復していたのに過ぎない…";
		break;
	case 2:
		m_currentChara = L"スライム";
		m_displayText = L"…え？\n森を修復していた？";
		break;
	case 3:
		m_currentChara = L"ディアボロ";
		m_displayText = L"そうだ、この森も、大地も――\nずっと前から、何者かによって荒らされていた。";
		break;
	case 4:
		m_displayText = L"我は……荒れ果てたこの土地を……\n再び、緑で満たすために……";
		break;
	case 5:
		m_displayText = L"力を……使っていたに……過ぎない……\n……それなのに……お前たちは……";
		break;
	case 6:
		m_displayText = L"……我の力が、弱まっていくのを感じる……\nこれもまた……運命なのだな……";
		break;
	case 7:
		m_displayText = L"……ならば、見届けるがいい。この世界の有様を\nそして……誰が、この世界を修復していたのかを……";
		break;
	case 8:
		m_currentChara = L"スライム";
		m_displayText = L"...まずい!ここもすぐ崩れる\nここにいたら危ない！　早く、あの場所に戻ろう！";
		break;
	}
}

void TextManager::EndText(int count)
{
	m_display = true;

	m_currentTextType = TextType::End;
	m_currentChara = L"スライム";
	switch (count)
	{
	case 0:
		m_displayText = L"...酷い有様になっちゃったね。\nでも、ディアボロはもういない。";
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
		m_gameOver = true;
		break;
	}
}

bool TextManager::GameClear()
{
	return m_gameClear;
}
bool TextManager::GameOver()
{
	return m_gameOver;
}
void TextManager::TypeWriter(InputManager&inputManager)
{
	m_fpsCounter++;
	if (m_fpsCounter >= m_displaySpeed)
	{
		m_fpsCounter = 0;
		if (m_displaytextLength < m_displayText.length())
		{
			m_displaytextLength++;
		}
	}
	m_currentdisplayText = m_displayText.substr(0, m_displaytextLength);
	if (m_displaytextLength == m_displayText.length())
	{
		m_isTyping = false;
	}
}
void TextManager::SkipText()
{
	m_displaytextLength = m_displayText.length();
	m_currentdisplayText = m_displayText;
	m_isTyping = false;
}
void TextManager::SetTyping()
{
	m_isTyping = true;
}

bool TextManager::GetTyping()const
{
	return m_isTyping;
}

void TextManager::SetDisplayTextLength()
{
	m_displaytextLength = 0;
}

void TextManager::DrawCooperatText()
{
	SetFontSize(35);
	DrawString(650,150,m_displayText.c_str(),GetColor(255, 255, 255),TRUE);
	SetFontSize(10);
}

void TextManager::CooperatText(CooperatList skill)
{
	switch (skill)
	{
	case CooperatList::None:
		m_displayText = L"無属性\n\n属性を持たない基本的な技。\n壁を壊すことが可能";
		break;

	case CooperatList::Fire:
		m_displayText = L"火属性\n\n炎の力を利用した技。\n燃やすことが可能";
		break;

	case CooperatList::Water:
		m_displayText = L"水属性\n\n水の力を利用した技。\n穴に水で満たすことが可能";
		break;

	case CooperatList::Grass:
		m_displayText = L"草属性\n\n自然の力を利用した技。\n枯れた地面を\n豊かにすることが可能";
		break;

	case CooperatList::Soil:
		m_displayText = L"土属性\n\n大地の力を利用した連携技。\n水の上に\n土を置くことが可能";
		break;

	case CooperatList::Wind:
		m_displayText = L"風属性\n\n風の力を利用した技。\n風で葉を飛ばすことが可能";
		break;

	case CooperatList::Darkness:
		m_displayText = L"闇属性\n\n闇の力を利用した技。\nすべてを飲み込む力で新た\nな道を切り開くことが可能";
		break;

	case CooperatList::SteamExplpsion:
		m_displayText = L"蒸界爆砕\n\n水と火の力を利用した\n強力な連携技。\nあたり一帯を吹き飛ばすこ\nとが可能";
		break;

	case CooperatList::FloorBreak:
		m_displayText = L"地殻崩壊\n\n大地を砕く強力な連携技。\n大きな穴をあけること\nが可能";
		break;

	case CooperatList::WaterFlows:
		m_displayText = L"蒼波\n\n激しい水流を発生させる\n強力な連携技。\nすべての穴を水で満たす\nことが可能";
		break;

	case CooperatList::GrawGrass:
		m_displayText = L"大地の恵み\n\n自然の力を利用した連携技。\n地面をより豊かにする\nことが可能";
		break;

	case CooperatList::Volcazation:
		m_displayText = L"灼界\n\n灼熱の力を利用した\n強力な連携技。\nすべての水を蒸発させる\nことが可能";
		break;

	default:
		m_displayText = L"";
		break;
	}
}