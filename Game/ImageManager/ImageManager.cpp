#include "pch.h"
#include "Game/ImageManager/ImageManager.h"

#include"Game/Maths/Vector2.h"
void ImageManager::LoadTextures()
{
	textures.push_back({ L"player1", LoadGraph(L"Resources/Textures/front.png") });
	textures.push_back({ L"player2", LoadGraph(L"Resources/Textures/back.png") });
	textures.push_back({ L"player3", LoadGraph(L"Resources/Textures/left.png") });
	textures.push_back({ L"player4", LoadGraph(L"Resources/Textures/right.png") });


	textures.push_back({ L"Slime", LoadGraph(L"Resources/Textures/slime.png") });
	textures.push_back({ L"Wolf", LoadGraph(L"Resources/Textures/wolf.png") });
	textures.push_back({ L"Dragon", LoadGraph(L"Resources/Textures/dragon.png") });
	textures.push_back({ L"Golem", LoadGraph(L"Resources/Textures/golem.png") });
	textures.push_back({ L"Fairy", LoadGraph(L"Resources/Textures/fairy.png") });

	textures.push_back({ L"N", LoadGraph(L"Resources/Textures/N.png") });
	textures.push_back({ L"M", LoadGraph(L"Resources/Textures/M.png") });

	textures.push_back({ L"forest",LoadGraph(L"Resources/Textures/forest.png") });
	textures.push_back({ L"plain",LoadGraph(L"Resources/Textures/plain.png") });
	textures.push_back({ L"river",LoadGraph(L"Resources/Textures/river.png") });
	textures.push_back({ L"desert",LoadGraph(L"Resources/Textures/desert.png") });
	textures.push_back({ L"volcano",LoadGraph(L"Resources/Textures/volcano.png") });
	textures.push_back({L"castle",LoadGraph(L"Resources/Textures/castle.png") });

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

void ImageManager::DrawDragon(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Dragon");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawGolem(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Golem");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}

void ImageManager::DrawFairy(Vector2 position, Vector2 size)
{
	int gh = GetTexture(L"Fairy");

	DrawExtendGraph(position.x, position.y, position.x + size.x, position.y + size.y, gh, TRUE);
}




////キー画像
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


///背景画像
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