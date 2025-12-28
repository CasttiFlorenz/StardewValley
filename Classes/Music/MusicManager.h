// MusicManager.h
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
