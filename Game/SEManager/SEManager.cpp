#include "pch.h"
#include "Game/SEManager/SEManager.h"

void SEManager::LoadSounds()
{
    //汎用
    soundEffects.push_back({ SoundList::Decision,LoadSoundMem(L"Resources/Sounds/decision.ogg") });
    soundEffects.push_back({ SoundList::Cancel,LoadSoundMem(L"Resources/Sounds/cancel.ogg") });
    soundEffects.push_back({ SoundList::Cursor,LoadSoundMem(L"Resources/Sounds/cursor.ogg") });

    //Title
    soundEffects.push_back({ SoundList::TitleBGM,LoadSoundMem(L"Resources/Sounds/titleBGM.ogg") });
    soundEffects.push_back({ SoundList::TitleSE,LoadSoundMem(L"Resources/Sounds/titleSE.ogg") });
    //Field
    soundEffects.push_back({ SoundList::FieldBGM,LoadSoundMem(L"Resources/Sounds/fieldBGM.ogg") });

    //Battle
    soundEffects.push_back({ SoundList::BattleBGM_Normal_1  ,LoadSoundMem(L"Resources/Sounds/battleBGM_1.ogg") });
    soundEffects.push_back({ SoundList::BattleBGM_Normal_2  ,LoadSoundMem(L"Resources/Sounds/battleBGM_2.ogg") });
    soundEffects.push_back({ SoundList::BattleBGM_Boss      ,LoadSoundMem(L"Resources/Sounds/battleBossBGM.ogg") });
    soundEffects.push_back({ SoundList::Run                 ,LoadSoundMem(L"Resources/Sounds/run.ogg" )});

}

int SEManager::GetSound(SoundList &soundlist)
{
    for (const auto& sound : soundEffects)
    {
        if (sound.soundlist == soundlist)
        {
            return sound.handle;
        }
    }
    return -1;
}

void SEManager::PlayTypeBackStart(SoundList soundlist)
{
    int sh = GetSound(soundlist);
    PlaySoundMem(sh, DX_PLAYTYPE_BACK, true);
}

void SEManager::SoundStop(SoundList soundlist)
{
    int sh = GetSound(soundlist);
    StopSoundMem(sh);
}

void SEManager::PlayTypeLoopStart(SoundList soundlist)
{
    int sh = GetSound(soundlist);
    PlaySoundMem(sh, DX_PLAYTYPE_LOOP, true);
}

