/****************************************************************
 * Project Name:  StardewValley
 * File Name:     WeatherManager.h
 * File Function: WeatherManager类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/
#ifndef __WEATHER_MANAGER_H__
#define __WEATHER_MANAGER_H__

#include "cocos2d.h"
#include "GameTime.h" 


enum class WeatherType {
    Sunny = 0,
    Rain = 1,
    Wind = 2, 
    Snow = 3
};

class WeatherManager : public cocos2d::Node {
public:
    static WeatherManager* getInstance();
    virtual bool init();

    void updateWeather(Season currentSeason);
    void setWeather(WeatherType type);

    // 更新天色
    void updateSkyColor(int hour, int minute);

    bool isRaining() const;
    WeatherType getCurrentWeather() const { return currentWeather; }

private:
    WeatherManager();
    ~WeatherManager();
    static WeatherManager* instance;

    WeatherType currentWeather;
    cocos2d::Sprite* weatherIcon; 
    Season currentSeason; // 记录当前季节

    // 视觉组件
    cocos2d::Layer* weatherLayer;
    cocos2d::LayerColor* overlayLayer;
    cocos2d::ParticleSystemQuad* particleEffect;

    // 内部方法
    void setVisuals(WeatherType type);

    void getTargetColor(int hour, int minute, cocos2d::Color3B& outColor, GLubyte& outOpacity);
};

#endif // __WEATHER_MANAGER_H__
