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
	void DrawSlime(Vector2 position, Vector2 size);
	void DrawWolf(Vector2 position, Vector2 size);
	void DrawDragon(Vector2 position, Vector2 size);
	void DrawN(Vector2 position, Vector2 size);
	void DrawM(Vector2 position,Vector2 size);
};

