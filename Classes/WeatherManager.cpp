/****************************************************************
 * Project Name:  StardewValley
 * File Name:     WeatherManager.cpp
 * File Function: WeatherManager类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/
#include "WeatherManager.h"

USING_NS_CC;

// 获取屏幕大小的宏
#ifndef WINSIZE
#define WINSIZE Director::getInstance()->getVisibleSize()
#endif

WeatherManager* WeatherManager::instance = nullptr;

WeatherManager* WeatherManager::getInstance() {
    if (!instance) {
        instance = new WeatherManager();
        if (instance && instance->init()) {
            instance->autorelease();
            instance->retain(); 
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;
}

void WeatherManager::destroyInstance() {
    if (instance) {
        instance->release();
        instance = nullptr;
    }
}

WeatherManager::WeatherManager()
    : currentWeather(WeatherType::Sunny)
    , currentSeason(Season::Spring)
    , weatherLayer(nullptr)
    , overlayLayer(nullptr)
    , particleEffect(nullptr) {
}

WeatherManager::~WeatherManager() {}

bool WeatherManager::init() {
    // 1. 滤镜层 - 确保在最顶层
    overlayLayer = LayerColor::create(Color4B(0, 0, 0, 0), WINSIZE.width, WINSIZE.height);
    overlayLayer->setCascadeOpacityEnabled(true); 
    this->addChild(overlayLayer, 9999);

    // 2. 天气粒子层 - 在滤镜下面
    weatherLayer = Layer::create();
    this->addChild(weatherLayer, 1);

    // 3. 确保图层是可见的
    overlayLayer->setVisible(true);

    // 4. 初始设置为白天
    overlayLayer->setColor(Color3B::BLACK);
    overlayLayer->setOpacity(0);

    return true;
}


void WeatherManager::updateWeather(Season season) {
    // 1. 记录当前季节，用于后续粒子系统（如风天使用花瓣还是落叶）的判断
    this->currentSeason = season;

    // 2. 核心随机数：生成一个 0 到 99 的随机整数。
    int chance = rand() % 100;

    // 3. 根据季节应用不同的天气概率分配规则
    switch (season) {
    case Season::Spring:
        // 春季概率分配：
        if (chance < 15) {
            currentWeather = WeatherType::Rain; // 15% 概率下雨 (chance 0-14)
        }
        else if (chance < 25) {
            currentWeather = WeatherType::Wind; // 10% 概率刮风/花瓣飘落 (chance 15-24)
        }
        else {
            currentWeather = WeatherType::Sunny; // 75% 概率晴天 (chance 25-99)
        }
        break;

    case Season::Fall:
        // 秋季概率分配：
        if (chance < 15) {
            currentWeather = WeatherType::Rain; // 15% 概率下雨 (chance 0-14)
        }
        else if (chance < 35) {
            currentWeather = WeatherType::Wind; // 20% 概率刮风/落叶 (chance 15-34)
        }
        else {
            currentWeather = WeatherType::Sunny; // 65% 概率晴天 (chance 35-99)
        }
        break;

    case Season::Summer:
        // 夏季概率分配：
        if (chance < 15) {
            currentWeather = WeatherType::Rain; // 15% 概率下雨 (chance 0-14)
        }
        else {
            currentWeather = WeatherType::Sunny; // 85% 概率晴天 (chance 15-99)
        }
        break;

    case Season::Winter:
        // 冬季概率分配：
        if (chance < 20) {
            currentWeather = WeatherType::Snow; // 20% 概率下雪 (chance 0-19)
        }
        else {
            currentWeather = WeatherType::Sunny; // 80% 概率晴天 (chance 20-99)
        }
        break;

    default:
        // 默认情况，如果季节未定义或出错，则设为晴天
        currentWeather = WeatherType::Sunny;
        break;
    }

    // 4. 根据确定的天气类型更新粒子效果和光照滤镜
    setVisuals(currentWeather);
}

void WeatherManager::setWeather(WeatherType type) {
    currentWeather = type;
    setVisuals(currentWeather);
}

bool WeatherManager::isRaining() const {
    return currentWeather == WeatherType::Rain;
}

// ==================== 视觉效果 (粒子系统) ====================

void WeatherManager::setVisuals(WeatherType type) {
    // 清理旧粒子
    if (particleEffect) {
        particleEffect->removeFromParent();
        particleEffect = nullptr;
    }

    switch (type) {
    case WeatherType::Sunny:
        // 晴天无特效
        break;
    case WeatherType::Wind:
    {
        // === 1. 纹理路径选择 ===
        std::string textureFile = (currentSeason == Season::Spring) ? "Weather/flower.png" : "Weather/leaf.png";

        // === 2. 纹理加载 ===
        auto textureCache = Director::getInstance()->getTextureCache();
        auto texture = textureCache->addImage(textureFile);

        // === 3. 创建粒子 ===
        particleEffect = ParticleSystemQuad::createWithTotalParticles(200); // 增加数量
        particleEffect->setTexture(texture);
        // 粒子存活时间
        particleEffect->setLife(4.0f);      // 粒子存活4秒
        particleEffect->setLifeVar(1.0f);   // 随机波动1秒

        particleEffect->setEmissionRate(particleEffect->getTotalParticles() / particleEffect->getLife());
        // === 4. 关键属性设置===
        particleEffect->setDuration(ParticleSystem::DURATION_INFINITY);
        particleEffect->setEmitterMode(ParticleSystem::Mode::GRAVITY);

        // 位置：从屏幕左上角和右上角生成
        particleEffect->setPosition(Vec2(WINSIZE.width / 2, WINSIZE.height + 50));
        particleEffect->setPosVar(Vec2(WINSIZE.width, 0));

        // 运动：向右下方飘落
        particleEffect->setGravity(Vec2(200, -100)); // 风力向右，重力向下
        particleEffect->setSpeed(150);
        particleEffect->setSpeedVar(50);
        particleEffect->setAngle(-45); // 右下方
        particleEffect->setAngleVar(20);

        // 大小
        particleEffect->setStartSize(25.0f);
        particleEffect->setStartSizeVar(10.0f);
        particleEffect->setEndSize(15.0f);

        // 颜色：根据季节设置明显颜色
        if (currentSeason == Season::Spring) {
            // 粉色花瓣
            particleEffect->setStartColor(Color4F(1.0f, 0.7f, 0.8f, 1.0f));
            particleEffect->setEndColor(Color4F(1.0f, 0.7f, 0.8f, 0.5f));
        }
        else {
            // 橙色/褐色落叶
            particleEffect->setStartColor(Color4F(1.0f, 0.6f, 0.2f, 1.0f));
            particleEffect->setEndColor(Color4F(0.8f, 0.5f, 0.1f, 0.5f));
        }

        weatherLayer->addChild(particleEffect);
        break;
    }

    case WeatherType::Rain:
    {
        particleEffect = ParticleSystemQuad::createWithTotalParticles(1500);

        // 1. 纹理
        auto texture = Director::getInstance()->getTextureCache()->addImage("Weather/raindrop.png");
        particleEffect->setTexture(texture);

        // 2. 基础设置
        particleEffect->setDuration(ParticleSystem::DURATION_INFINITY);
        particleEffect->setEmitterMode(ParticleSystem::Mode::GRAVITY);

        // 3. 位置和范围
        particleEffect->setPosition(Vec2(WINSIZE.width / 2, WINSIZE.height + 100));
        particleEffect->setPosVar(Vec2(WINSIZE.width / 2, 20));

        // 4. 物理参数
        particleEffect->setGravity(Vec2(40, -1000));
        particleEffect->setSpeed(50);
        particleEffect->setSpeedVar(20);
        particleEffect->setEmissionRate(100.0f);

        // 5. 生命周期
        particleEffect->setLife(1.2f);
        particleEffect->setLifeVar(0.3f);

        // 6. 颜色
        particleEffect->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 0.9f));
        particleEffect->setEndColor(Color4F(0.85f, 0.9f, 1.0f, 0.7f));

        // 7. 大小（控制雨滴形状）
        particleEffect->setStartSize(20.0f);
        particleEffect->setStartSizeVar(5.0f);
        particleEffect->setEndSize(15.0f);

        // 8. 角度（垂直向下）
        particleEffect->setAngle(270);
        particleEffect->setAngleVar(15);

        // 9. 混合模式（更亮）
        particleEffect->setBlendAdditive(true);

        weatherLayer->addChild(particleEffect);
        break;
    }
    case WeatherType::Snow:
    {
        particleEffect = ParticleSnow::create();
        particleEffect->setGravity(Vec2(0, -3));

        // 设置速度
        particleEffect->setSpeed(20.0f);
        particleEffect->setSpeedVar(3.0f);

        particleEffect->setPosition(Vec2(WINSIZE.width / 2, WINSIZE.height));
        weatherLayer->addChild(particleEffect);
        break;
    }
    }
}
// ==================== 天色与光照 ====================
void WeatherManager::updateSkyColor(int hour, int minute) {
    // 特殊天气优先处理 
    if (currentWeather == WeatherType::Rain) {
        if (overlayLayer) {
            overlayLayer->stopAllActions();
            overlayLayer->setColor(Color3B(30, 40, 60)); 
            overlayLayer->setOpacity(150);
        }
        return;
    }

    // 获取目标颜色
    Color3B targetColor;
    GLubyte targetOpacity;

    getTargetColor(hour, minute, targetColor, targetOpacity);

    // 应用颜色到图层
    if (overlayLayer) {
        overlayLayer->stopAllActions();
        overlayLayer->setColor(targetColor);
        overlayLayer->setOpacity(targetOpacity);
    }
}
void WeatherManager::getTargetColor(int hour, int minute, Color3B& outColor, GLubyte& outOpacity) {
    int totalMinutes = hour * 60 + minute;


    // --- 1. 深夜 (0:00 - 2:00) ---
    if (totalMinutes >= 0 && totalMinutes < 2 * 60) {
        float progress = (float)totalMinutes / (2.0f * 60);

        outColor.r = static_cast<GLubyte>(5 + (10 * progress));
        outColor.g = static_cast<GLubyte>(10 + (10 * progress));
        outColor.b = static_cast<GLubyte>(30 + (20 * progress));

        outOpacity = static_cast<GLubyte>(210 - (20 * progress));
        return;
    }

    // --- 2. 黎明前 (2:00 - 4:00)  ---
    if (totalMinutes >= 2 * 60 && totalMinutes < 4 * 60) {
        float progress = (float)(totalMinutes - 2 * 60) / (2.0f * 60);

        outColor.r = static_cast<GLubyte>(15 + (135 * progress)); 
        outColor.g = static_cast<GLubyte>(20 + (180 * progress)); 
        outColor.b = static_cast<GLubyte>(50 + (205 * progress)); 
        outOpacity = static_cast<GLubyte>(190 + (10 * progress));
        return;
    }

    // --- 3. 早晨 (4:00 - 7:00) ---
    if (totalMinutes >= 4 * 60 && totalMinutes < 7 * 60) {
        float progress = (float)(totalMinutes - 4 * 60) / (3.0f * 60);
        outColor = Color3B(150, 200, 255);
        outOpacity = static_cast<GLubyte>(200 * (1.0f - progress));
        return;
    }

    // --- 4. 白天 (7:00 - 18:00) ---
    if (totalMinutes >= 7 * 60 && totalMinutes < 18 * 60) {
        outColor = Color3B(0, 0, 0); 
        outOpacity = 0;
        return;
    }

    // --- 5. 黄昏 (18:00 - 20:00) ---
    if (totalMinutes >= 18 * 60 && totalMinutes < 20 * 60) {
        float progress = (float)(totalMinutes - 18 * 60) / (2.0f * 60);

        outColor = Color3B(15, 20, 45); 
        outOpacity = static_cast<GLubyte>(0 + (160 * progress));
        return;
    }

    // --- 6. 夜晚 (20:00 - 24:00) ---
    if (totalMinutes >= 20 * 60 && totalMinutes < 24 * 60) {
        float progress = (float)(totalMinutes - 20 * 60) / (4.0f * 60);

        outColor.r = static_cast<GLubyte>(15 - (10 * progress));
        outColor.g = static_cast<GLubyte>(20 - (10 * progress));
        outColor.b = static_cast<GLubyte>(45 - (15 * progress)); 
        outOpacity = static_cast<GLubyte>(160 + (50 * progress));
        return;
    }
}
