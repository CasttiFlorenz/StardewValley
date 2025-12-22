// MusicManager.cpp
#include "MusicManager.h"
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

MusicManager* MusicManager::instance = nullptr;

MusicManager::MusicManager() {
    volume = 0.01f;
    effectVolume = 1.0f;
    currentMusic = "";
    isPreloaded = false; // 添加预加载状态标记
}

MusicManager* MusicManager::getInstance() {
    if (instance == nullptr) {
        instance = new MusicManager();
    }
    return instance;
}

void MusicManager::preloadMusic() {
    if (isPreloaded) {
        CCLOG("音乐已经预加载过了");
        return;
    }

    CCLOG("开始预加载音乐...");

    std::vector<std::string> musicFiles = {
        "music/FarmHouse.mp3",
        "music/Farm.mp3",
        "music/Town.mp3",
        "music/click.mp3",
    };

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
    SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);

    CCLOG("音乐预加载完成");
}

void MusicManager::playButtonClick() {
    SimpleAudioEngine::getInstance()->playEffect("Music/click.mp3");
    CCLOG("播放按钮点击音效");
}

void MusicManager::playMusicForMap(const std::string& _mapName) {
    std::string musicFile;
    std::string mapName = _mapName;

    if (mapName == "Farm") {
        musicFile = "Music/Farm.mp3";
    }
    else if (mapName == "Town") {
        musicFile = "Music/Town.mp3";
    }
    else if (mapName == "FarmHouse") {
        musicFile = "Music/FarmHouse.mp3";
    }

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);

    if (currentMusic == musicFile) {
        if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
            CCLOG("音乐已经在播放，跳过");
            return;
        }
    }

    if (!currentMusic.empty() && SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
        CCLOG("停止当前音乐: %s", currentMusic.c_str());
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }

    CCLOG("开始播放: %s", musicFile.c_str());

    // 添加延迟以避免卡顿
    cocos2d::Director::getInstance()->getScheduler()->schedule([this, musicFile](float dt) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(musicFile.c_str(), true);
        }, this, 0.0f, 0, 0.05f, false, "play_music");

    currentMusic = musicFile;
}

void MusicManager::stopMusic() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    currentMusic = "";
}

void MusicManager::setMusicVolume(float vol) {
    volume = vol;
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

void MusicManager::setEffectVolume(float vol) {
    effectVolume = vol;
    if (effectVolume < 0.0f) effectVolume = 0.0f;
    if (effectVolume > 1.0f) effectVolume = 1.0f;

    SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);
}

void MusicManager::pauseMusic() {
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void MusicManager::resumeMusic() {
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

bool MusicManager::isMusicPlaying() {
    return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}