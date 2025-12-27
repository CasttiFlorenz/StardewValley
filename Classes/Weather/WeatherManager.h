/****************************************************************
 * Project Name:  StardewValley
 * File Name:     WeatherManager.h
 * File Function: WeatherManager类的声明
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/
#ifndef WEATHER_MANAGER_H
#define WEATHER_MANAGER_H

#include "cocos2d.h"
#include "Constant.h" 

class WeatherManager : public cocos2d::Node {
public:
    // 单例获取与销毁
    static WeatherManager* getInstance();
    static void destroyInstance();

    // 初始化节点
    virtual bool init() override;

    // 更新天气逻辑
    void updateWeather(Season season);

    // 强制设置天气
    void setWeather(WeatherType type);

    // 判断是否下雨
    bool isRaining() const;

    // 获取当前天气
    WeatherType getCurrentWeather() const;

    // 更新天色光照
    void updateSkyColor(int hour, int minute);

private:
    WeatherManager();
    virtual ~WeatherManager();

    // 设置粒子视觉效果
    void setVisuals(WeatherType type);

    // 计算目标颜色
    void getTargetColor(int hour, int minute, cocos2d::Color3B& outColor, GLubyte& outOpacity);

    // 获取纹理（含保底）
    cocos2d::Texture2D* getTextureOrFallback(const std::string& filename);

private:
    static WeatherManager* _instance;

    WeatherType _currentWeather;
    Season _currentSeason;

    cocos2d::LayerColor* _overlayLayer;
    cocos2d::Layer* _weatherLayer;
    cocos2d::ParticleSystem* _particleEffect;
};

#endif //