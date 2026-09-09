#pragma once
class SEManager
{
public:
	enum class SoundList
	{
		//Attack
		Fire,Water,Grass,Wind,Darkness,
		SteamExplosion,FloorBreak,WaterFlows,GrawGrass,Volcazation,Defense,

		Decision,Cancel,Cursor,
		Lose,/*GameOver*/Break,/*Map->Break*/

		//Title
		TitleBGM,
		TitleSE,
		
		//Field
		FieldBGM,
		FieldSE,
		
		//Battle
		BattleBGM_Normal_1,BattleBGM_Normal_2,BattleBGM_Boss,
		Run,Win,
	};
private:
	struct SoundEffects
	{
		SoundList soundlist;
		int handle;
	};
	std::vector<SoundEffects>soundEffects;
public:
	void LoadSounds();
	int GetSound(SoundList &soundlist);

	void PlayTypeBackStart(SoundList soundlist);
	void PlayTypeLoopStart(SoundList soundlist);
	
	void SoundStop(SoundList soundlist);
};

