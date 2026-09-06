#pragma once
class SEManager
{
public:
	enum class SoundList
	{
		Decision,Cancel,Cursor,
		
		//Title
		TitleBGM,
		TitleSE,
		
		//Field
		FieldBGM,
		FieldSE,
		
		//Battle
		BattleBGM_Normal_1,BattleBGM_Normal_2,BattleBGM_Boss,
		Run,
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

