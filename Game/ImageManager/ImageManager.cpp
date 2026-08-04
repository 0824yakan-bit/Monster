#include "pch.h"
#include "Game/ImageManager/ImageManager.h"

#include"Game/Maths/Vector2.h"
void ImageManager::LoadTextures()
{
	textures.push_back({ L"Slime", LoadGraph(L"Resources/Textures/slime.png") });
	textures.push_back({ L"Wolf", LoadGraph(L"Resources/Textures/wolf.png") });
	textures.push_back({ L"Dragon", LoadGraph(L"Resources/Textures/dragon.png") });
	textures.push_back({ L"N", LoadGraph(L"Resources/Textures/N.png") });
	textures.push_back({ L"M", LoadGraph(L"Resources/Textures/M.png") });

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