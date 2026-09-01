#include "pch.h"
#include "Game/ImageManager/ImageManager.h"

#include"Game/Maths/Vector2.h"
void ImageManager::LoadTextures()
{
	textures.push_back({ L"titletext", LoadGraph(L"Resources/Textures/titletext.png") });
	textures.push_back({ L"title", LoadGraph(L"Resources/Textures/title.png") });


	textures.push_back({ L"player1", LoadGraph(L"Resources/Textures/front.png") });
	textures.push_back({ L"player2", LoadGraph(L"Resources/Textures/back.png") });
	textures.push_back({ L"player3", LoadGraph(L"Resources/Textures/left.png") });
	textures.push_back({ L"player4", LoadGraph(L"Resources/Textures/right.png") });


	textures.push_back({ L"Slime", LoadGraph(L"Resources/Textures/slime.png") });
	textures.push_back({ L"Wolf", LoadGraph(L"Resources/Textures/wolf.png") });
	textures.push_back({ L"Fairy", LoadGraph(L"Resources/Textures/fairy.png") });
	textures.push_back({ L"Turtle", LoadGraph(L"Resources/Textures/turtle.png") });
	textures.push_back({ L"Mole", LoadGraph(L"Resources/Textures/mole.png") });
	textures.push_back({ L"Fox", LoadGraph(L"Resources/Textures/fox.png") });
	textures.push_back({ L"Golem", LoadGraph(L"Resources/Textures/golem.png") });
	textures.push_back({ L"Phoenix", LoadGraph(L"Resources/Textures/phoenix.png") });
	textures.push_back({ L"Dragon", LoadGraph(L"Resources/Textures/dragon.png") });
	textures.push_back({ L"Daemon", LoadGraph(L"Resources/Textures/daemon.png") });

	textures.push_back({ L"N", LoadGraph(L"Resources/Textures/N.png") });
	textures.push_back({ L"M", LoadGraph(L"Resources/Textures/M.png") });
	textures.push_back({ L"key", LoadGraph(L"Resources/Textures/key.png") });
	textures.push_back({ L"controller", LoadGraph(L"Resources/Textures/controller.png") });

	textures.push_back({ L"forest",LoadGraph(L"Resources/Textures/forest.png") });
	textures.push_back({ L"plain",LoadGraph(L"Resources/Textures/plain.png") });
	textures.push_back({ L"river",LoadGraph(L"Resources/Textures/river.png") });
	textures.push_back({ L"desert",LoadGraph(L"Resources/Textures/desert.png") });
	textures.push_back({ L"volcano",LoadGraph(L"Resources/Textures/volcano.png") });
	textures.push_back({L"castle",LoadGraph(L"Resources/Textures/castle.png") });

	textures.push_back({ L"commandcursor",LoadGraph(L"Resources/Textures/command.png") });
	textures.push_back({ L"commandbox1",LoadGraph(L"Resources/Textures/commandbox1.png") });
	textures.push_back({ L"commandbox2",LoadGraph(L"Resources/Textures/commandbox2.png") });
	
	textures.push_back({ L"fire",LoadGraph(L"Resources/Textures/fire.png") });
	textures.push_back({ L"water",LoadGraph(L"Resources/Textures/water.png") });
	textures.push_back({ L"grass",LoadGraph(L"Resources/Textures/grass.png") });
	textures.push_back({ L"soil",LoadGraph(L"Resources/Textures/soil.png") });
	textures.push_back({ L"wind",LoadGraph(L"Resources/Textures/wind.png") });
	textures.push_back({ L"darkness",LoadGraph(L"Resources/Textures/darkness.png") });
	textures.push_back({ L"steamexplosion",LoadGraph(L"Resources/Textures/steamexplosion.png") });
	textures.push_back({ L"waterflows",LoadGraph(L"Resources/Textures/waterflows.png") });
	textures.push_back({ L"floorbreak",LoadGraph(L"Resources/Textures/floorbreak.png") });
	textures.push_back({ L"volcazation",LoadGraph(L"Resources/Textures/volcazation.png") });
	textures.push_back({ L"growgrass",LoadGraph(L"Resources/Textures/growgrass.png") });



}

int ImageManager::GetTexture(const std::wstring& name)
{
	for (const auto& tex : textures)
	{
		if (tex.name == name)
		{
			return tex.handle;
		}
	}
	return-1;
}
///タイトル画像
void ImageManager::DrawTitleText(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"titletext");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawTitle(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"title");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

/// プレイヤー画像
void ImageManager::DrawPlayer1(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"player1");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawPlayer2(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"player2");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawPlayer3(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"player3");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawPlayer4(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"player4");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

///モンスター画像
void ImageManager::DrawSlime(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Slime");

	DrawExtendGraph(position.x,position.y,position.x + size.x,position.y + size.y,gh,TRUE);
}

void ImageManager::DrawWolf(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Wolf");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawFairy(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Fairy");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawTurtle(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Turtle");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawMole(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Mole");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawFox(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Fox");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawGolem(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Golem");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawPhoenix(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Phoenix");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawDragon(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Dragon");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawDaemon(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Daemon");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}



////フィールドシーンUI画像
void ImageManager::DrawN(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"N");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawM(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"M");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawKey(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"key");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawController(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"controller");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}


///バトルシーン背景画像
void ImageManager::DrawForest(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"forest");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawPlain(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"plain");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawRiver(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"river");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawDesrt(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"desert");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawVolcano(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"volcano");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawCastle(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"castle");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}


//バトルシーンUI画像
void ImageManager::DrawCommandCursor(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"commandcursor");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawCommandbox1(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"commandbox1");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawCommandbox2(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"commandbox2");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

//バトルシーンエフェクト画像
void ImageManager::DrawFire(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"fire");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawWater(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"water");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawGrass(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"grass");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawSoil(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"soil");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawWind(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"wind");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawDarkness(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"darkness");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawSteamexplosion(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"steamexplosion");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawWaterflows(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"waterflows");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawFloorBreak(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"floorbreak");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawVolcazation(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"volcazation");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}
void ImageManager::DrawGrowgrass(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"growgrass");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}