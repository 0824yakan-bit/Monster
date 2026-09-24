#include"pch.h"
#include"Game/Scene/Transition.h"
#include"Game/Screen.h"
void Transition::Initialize()
{
	m_posX.Left		= Screen::LEFT;
	m_posX.Center	= Screen::WIDTH / 2;
	m_posX.Right	= Screen::WIDTH;
	m_posY.Top		= Screen::TOP;
	m_posY.Center	= Screen::HEIGHT / 2;
	m_posY.Bottom	= Screen::BOTTOM;
}
void Transition::TitletoPlayOut(float faderate)
{
	//描画に使用するアルファ値を計算する
	const int alpha = static_cast<int>(255 * faderate);

	//アルファブレンドを設定し、画面を覆う四角形を描画する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, Colors::WHITE, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Transition::TitletoPlayIn(float faderate)
{
	//FadeIn:
	//faderate=1.0→0.0
	//
	//円:
	//半径0→MAX_RADIUS
	const float radiusfloat = MAX_RADIUS * (1.0f - faderate);

	const int centerX = 100;
	const int centerY = 200;
	const int radius = static_cast<int>(radiusfloat);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//半径0なら画面全体を白くする
	if (radius <= 0)
	{
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, Colors::WHITE, TRUE);
		return;
	}

	//円の外側だけ白く描画する
	for (int y = 0;y < Screen::HEIGHT;y++)
	{
		const int dy = y - centerY;

		//円より上・下なら、その行は全部白
		if (abs(dy) >= radius)
		{
			DrawBox(0, y, Screen::WIDTH, y + 1, Colors::WHITE, TRUE);
			continue;
		}

		//円の横方向の長さを計算
		const float x = sqrtf(static_cast<float>(radius * radius - dy * dy));

		const int left = centerX - static_cast<int>(x);
		const int right = centerX + static_cast<int>(x);

		//円の左側を白くする
		if (left > 0)
		{
			DrawBox(0, y, left, y + 1, Colors::WHITE, TRUE);
		}

		//円の右側を白くする
		if (right < Screen::WIDTH)
		{
			DrawBox(right, y, Screen::WIDTH, y + 1, Colors::WHITE, TRUE);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



void Transition::PlaytoTitleOut(float faderate)
{
}

void Transition::PlaytoTitleIn(float faderate)
{
}

void Transition::FieldtoBattle_1Out(float faderate)
{
float CenterToLeft = m_posX.Center - (m_posX.Center * faderate);
	DrawBox(m_posX.Center, m_posY.Center, CenterToLeft, m_posY.Top, GetColor(0, 0, 0), TRUE);
float CenterToBottom = m_posY.Center + (m_posY.Center * faderate);
	DrawBox(m_posX.Center, m_posY.Center, m_posX.Left, CenterToBottom, GetColor(0, 0, 0), TRUE);
float CenterToRighit = m_posX.Center + (m_posX.Center * faderate);
	DrawBox(m_posX.Center, m_posY.Center, CenterToRighit, m_posY.Bottom, GetColor(0, 0, 0), TRUE);
float CenterToTop = m_posY.Center - (m_posY.Center * faderate);
	DrawBox(m_posX.Center, m_posY.Center, m_posX.Right,CenterToTop, GetColor(0, 0, 0), TRUE);

}

void Transition::FieldtoBattle_2Out(float faderate)
{
	int center = Screen::WIDTH / 2;
	int width = static_cast<int>(center * (1.0-faderate));
	DrawBox(0, 0,center - width,Screen::HEIGHT,GetColor(0, 0, 0),TRUE);
	DrawBox(center + width, 0,Screen::WIDTH,Screen::HEIGHT,GetColor(0, 0, 0),TRUE);
}

void Transition::FieldtoBattle_3Out(float faderate)
{
	int height = static_cast<int>(Screen::HEIGHT * faderate);
	DrawBox(0,0,Screen::WIDTH,height,GetColor(0, 0, 0),TRUE);
}

void Transition::FieldtoBattle_4Out(float faderate)
{
	int w = Screen::WIDTH;
	int h = Screen::HEIGHT;

	const int boxSize = 80;

	int columnCount = (w + boxSize - 1) / boxSize;
	int rowCount = (h + boxSize - 1) / boxSize;

	int totalBoxCount = columnCount * rowCount;

	// faderate = 0.0f → 何もない
	// faderate = 1.0f → 画面全体が黒
	int currentBoxCount =
		static_cast<int>(totalBoxCount * faderate);

	for (int i = 0; i < currentBoxCount; i++)
	{
		int column = i % columnCount;
		int row = i / columnCount;

		int x = column * boxSize;
		int y = row * boxSize;

		int right = x + boxSize;
		int bottom = y + boxSize;

		if (right > w)
		{
			right = w;
		}

		if (bottom > h)
		{
			bottom = h;
		}

		DrawBox(
			x,
			y,
			right,
			bottom,
			GetColor(0, 0, 0),
			TRUE
		);
	}
}

void Transition::FieldtoBattle_5Out(float faderate)
{
	int cx = Screen::WIDTH / 2;
	int cy = Screen::HEIGHT / 2;

	int halfW = static_cast<int>((Screen::WIDTH / 2) * (1.0f - faderate));
	int halfH = static_cast<int>((Screen::HEIGHT / 2) * (1.0f - faderate));

	int left = cx - halfW;
	int right = cx + halfW;
	int top = cy - halfH;
	int bottom = cy + halfH;

	// 上
	DrawBox(0, 0,Screen::WIDTH, top,GetColor(0, 0, 0),TRUE);

	// 下
	DrawBox(0, bottom,Screen::WIDTH, Screen::HEIGHT,GetColor(0, 0, 0),TRUE);

	// 左
	DrawBox(0, top,left, bottom,GetColor(0, 0, 0),TRUE);

	// 右
	DrawBox(right, top,Screen::WIDTH, bottom,GetColor(0, 0, 0),TRUE);
}

void Transition::FieldtoBattle_1In(float faderate)
{
	int color = GetColor(0, 0, 0);

	float t = 1.0f - faderate;

	// SmoothStep
	float ease = t * t * (3.0f - 2.0f * t);

	float cx = m_posX.Center;
	float cy = m_posY.Center;

	float w = Screen::WIDTH;
	float h = Screen::HEIGHT;

	float moveX = w * ease;
	float moveY = h * ease;

	DrawBox(
		0,
		0,
		static_cast<int>(cx - moveX / 2),
		Screen::HEIGHT,
		color,
		TRUE
	);

	DrawBox(
		static_cast<int>(cx + moveX / 2),
		0,
		Screen::WIDTH,
		Screen::HEIGHT,
		color,
		TRUE
	);
}

void Transition::FieldtoBattle_2In(float faderate)
{
	int center = Screen::WIDTH / 2;

	int width = static_cast<int>(center * (1.0f - faderate));

	// 左側
	DrawBox(0,0,center - width,Screen::HEIGHT,GetColor(0, 0, 0),TRUE);

	// 右側
	DrawBox(center + width,0,Screen::WIDTH,Screen::HEIGHT,GetColor(0, 0, 0),TRUE);
}

void Transition::FieldtoBattle_3In(float faderate)
{
	int height = static_cast<int>(Screen::HEIGHT * faderate);

	DrawBox(0,0,Screen::WIDTH,height,GetColor(0, 0, 0),TRUE);
}

void Transition::FieldtoBattle_4In(float faderate)
{
	int w = Screen::WIDTH;
	int h = Screen::HEIGHT;

	const int boxSize = 80;

	int columnCount = (w + boxSize - 1) / boxSize;
	int rowCount = (h + boxSize - 1) / boxSize;

	int totalBoxCount = columnCount * rowCount;

	// 何個のブロックを表示するか
	int currentBoxCount =
		static_cast<int>(totalBoxCount * faderate);

	// 上から順番にブロックを配置
	for (int i = 0; i < currentBoxCount; i++)
	{
		int x = (i % columnCount) * boxSize;
		int y = (i / columnCount) * boxSize;

		int right = x + boxSize;
		int bottom = y + boxSize;

		// 画面外にはみ出さないようにする
		if (right > w)
		{
			right = w;
		}

		if (bottom > h)
		{
			bottom = h;
		}

		DrawBox(
			x,
			y,
			right,
			bottom,
			GetColor(0, 0, 0),
			TRUE
		);
	}
}

void Transition::FieldtoBattle_5In(float faderate)
{
	int cx = Screen::WIDTH / 2;
	int cy = Screen::HEIGHT / 2;

	// faderate = 0 → 画面全体が黒
	// faderate = 1 → 中央だけ黒
	float rate = 1.0f - faderate;

	int halfW = static_cast<int>(
		(Screen::WIDTH / 2) * rate
		);

	int halfH = static_cast<int>(
		(Screen::HEIGHT / 2) * rate
		);

	int left = cx - halfW;
	int right = cx + halfW;
	int top = cy - halfH;
	int bottom = cy + halfH;

	// 上
	DrawBox(
		0, 0,
		Screen::WIDTH, top,
		GetColor(0, 0, 0),
		TRUE
	);

	// 下
	DrawBox(
		0, bottom,
		Screen::WIDTH, Screen::HEIGHT,
		GetColor(0, 0, 0),
		TRUE
	);

	// 左
	DrawBox(
		0, top,
		left, bottom,
		GetColor(0, 0, 0),
		TRUE
	);

	// 右
	DrawBox(
		right, top,
		Screen::WIDTH, bottom,
		GetColor(0, 0, 0),
		TRUE
	);
}

void Transition::BattletoField_1Out(float faderate)
{
}

void Transition::BattletoField_2Out(float faderate)
{
}

void Transition::BattletoField_1In(float faderate)
{
}

void Transition::BattletoField_2In(float faderate)
{
}
