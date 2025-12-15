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
#include "../Time/GameTime.h" // 确保引用了你的 GameTime 类定义

// 定义天气类型
enum class WeatherType {
    Sunny = 0,
    Rain,
    Wind,
    Snow
};

class WeatherManager : public cocos2d::Node {
public:
    // 单例访问
    static WeatherManager* getInstance();
    static void destroyInstance();

    // 初始化
    virtual bool init() override;

    // 核心逻辑：更新天气 (每天早上调用)
    void updateWeather(Season currentSeason);

    // 手动设置天气 (测试用)
    void setWeather(WeatherType type);

    // 更新天色滤镜 (每10分钟调用)
    void updateSkyColor(int hour, int minute);

    // 获取当前状态
    bool isRaining() const;
    WeatherType getCurrentWeather() const { return currentWeather; }

private:
    WeatherManager();
    ~WeatherManager();
    static WeatherManager* instance;

    // 数据
    WeatherType currentWeather;
    Season currentSeason;

    // 视觉组件
    cocos2d::Layer* weatherLayer;       // 粒子层
    cocos2d::LayerColor* overlayLayer;  // 滤镜层
    cocos2d::ParticleSystemQuad* particleEffect; // 当前粒子

    // 内部方法
    void setVisuals(WeatherType type);
    void WeatherManager::getTargetColor(int hour, int minute, cocos2d::Color3B& outColor, GLubyte& outOpacity);
};

#endif // __WEATHER_MANAGER_H__