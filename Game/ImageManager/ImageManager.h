#pragma once
#include<vector>
#include<string>
class Vector2;
class ImageManager
{
private:
	struct Texture
	{
		std::wstring name;
		int handle;
	};
	std::vector<Texture>textures;

public:
	void LoadTextures();
	int GetTexture(const std::wstring& name);

	void DrawPlayer1(Vector2 position, Vector2 size);
	void DrawPlayer2(Vector2 position, Vector2 size);
	void DrawPlayer3(Vector2 position, Vector2 size);
	void DrawPlayer4(Vector2 position, Vector2 size);


	void DrawSlime(Vector2 position, Vector2 size);
	void DrawWolf(Vector2 position, Vector2 size);
	void DrawDragon(Vector2 position, Vector2 size);
	void DrawGolem(Vector2 position, Vector2 size);
	void DrawFairy(Vector2 position, Vector2 size);


	void DrawN(Vector2 position, Vector2 size);
	void DrawM(Vector2 position,Vector2 size);
};

