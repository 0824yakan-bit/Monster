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

	void DrawTitleText(Vector2 position, Vector2 size);
	void DrawTitle(Vector2 position, Vector2 size);

	void DrawPlayer1(Vector2 position, Vector2 size);
	void DrawPlayer2(Vector2 position, Vector2 size);
	void DrawPlayer3(Vector2 position, Vector2 size);
	void DrawPlayer4(Vector2 position, Vector2 size);


	void DrawSlime(Vector2 position, Vector2 size);
	void DrawWolf(Vector2 position, Vector2 size);
	void DrawFairy(Vector2 position, Vector2 size);
	void DrawTurtle(Vector2 position, Vector2 size);
	void DrawMole(Vector2 position, Vector2 size);
	void DrawFox(Vector2 position, Vector2 size);
	void DrawGolem(Vector2 position, Vector2 size);
	void DrawPhoenix(Vector2 position, Vector2 size);
	void DrawDragon(Vector2 position, Vector2 size);
	void DrawDaemon(Vector2 position, Vector2 size);


	void DrawN(Vector2 position, Vector2 size);
	void DrawM(Vector2 position, Vector2 size);
	void DrawKey(Vector2 position, Vector2 size);
	void DrawController(Vector2 position,Vector2 size);

	void DrawForest(Vector2 position, Vector2 size);
	void DrawPlain(Vector2 position, Vector2 size);
	void DrawRiver(Vector2 position, Vector2 size);
	void DrawDesrt(Vector2 position, Vector2 size);
	void DrawVolcano(Vector2 position, Vector2 size);
	void DrawCastle(Vector2 position, Vector2 size);
	
	void DrawCommandCursor(Vector2 position, Vector2 size);
	void DrawCommandbox1(Vector2 position, Vector2 size);
	void DrawCommandbox2(Vector2 position, Vector2 size);

	void DrawFire(Vector2 position, Vector2 size);
	void DrawWater(Vector2 position, Vector2 size);
	void DrawGrass(Vector2 position, Vector2 size);
	void DrawSoil(Vector2 position, Vector2 size);
	void DrawWind(Vector2 position, Vector2 size);
	void DrawDarkness(Vector2 position, Vector2 size);
	void DrawSteamexplosion(Vector2 position, Vector2 size);
	void DrawWaterflows(Vector2 position, Vector2 size);
	void DrawFloorBreak(Vector2 position, Vector2 size);
	void DrawVolcazation(Vector2 position, Vector2 size);
	void DrawGrowgrass(Vector2 position, Vector2 size);



};

