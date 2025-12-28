/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MusicManager.h
 * File Function: MusicManager类的实现
 * Author:        蔡锦慧
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/
#ifndef __MUSIC_MANAGER_H__
#define __MUSIC_MANAGER_H__

#include "cocos2d.h"

class MusicManager {
private:
    static MusicManager* instance;

    std::string _currentMusic;

    float _volume;
    float _effectVolume;

    bool _isPreloaded;

    MusicManager();

public:
    static MusicManager* getInstance();
    static void destroyInstance();

    void preloadMusic();

    void playMusicForMap(const std::string& _mapName);

    void stopMusic();

    void setMusicVolume(float vol);
    void setEffectVolume(float vol);

    void playButtonClick();

    void pauseMusic();

    void resumeMusic();

    bool isMusicPlaying();
};


#endif // __MUSIC_MANAGER_H__


