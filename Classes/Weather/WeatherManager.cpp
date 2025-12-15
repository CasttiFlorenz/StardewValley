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
            instance->retain(); // 建议retain防止意外释放
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
    overlayLayer->setCascadeOpacityEnabled(true); // 启用透明度级联
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

// ==================== 逻辑控制 ====================

void WeatherManager::updateWeather(Season season) {
    // 1. 记录当前季节，用于后续粒子系统（如风天使用花瓣还是落叶）的判断
    this->currentSeason = season;

    // 2. 核心随机数：生成一个 0 到 99 的随机整数。
    //    这个数字被视为 100% 的概率空间，用于划分不同的天气区间。
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

    // 4. 调用 setVisuals 函数，根据确定的天气类型更新粒子效果和光照滤镜
    setVisuals(currentWeather);
}

void WeatherManager::setWeather(WeatherType type) {
    currentWeather = type;
    setVisuals(currentWeather);
}

bool WeatherManager::isRaining() const {
    return currentWeather == WeatherType::Rain;
}
Texture2D* getTextureOrFallback(const std::string& filename) {
    auto textureCache = Director::getInstance()->getTextureCache();
    // 1. 尝试加载图片
    auto texture = textureCache->addImage(filename);

    // 2. 如果加载成功，直接返回
    if (texture) return texture;

    // 3. 如果失败，创建一个 8x8 的白色方块纹理（保底方案）
    CCLOG("Warning: Texture %s not found. Using fallback.", filename.c_str());

    // 检查是否已经创建过保底纹理
    texture = textureCache->getTextureForKey("fallback_white_box");
    if (texture) return texture;

    // 创建纯色数据
    const int w = 8, h = 8;
    unsigned char* data = new unsigned char[w * h * 4];
    for (int i = 0; i < w * h * 4; i++) data[i] = 255; // 全白

    Image* image = new Image();
    image->initWithRawData(data, w * h * 4, w, h, 8);
    texture = textureCache->addImage(image, "fallback_white_box");

    delete[] data;
    image->release();
    return texture;
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

        // === 2. 使用保底纹理加载 ===
        auto texture = getTextureOrFallback(textureFile);

        // === 3. 创建粒子 ===
        particleEffect = ParticleSystemQuad::createWithTotalParticles(200); // 增加数量
        particleEffect->setTexture(texture);
        // 1. 设置粒子存活时间
        particleEffect->setLife(4.0f);      // 粒子存活4秒
        particleEffect->setLifeVar(1.0f);   // 随机波动1秒

        // 2. 设置发射速率 (总粒子数 / 存活时间 = 均匀发射)
        particleEffect->setEmissionRate(particleEffect->getTotalParticles() / particleEffect->getLife());
        // === 4. 关键属性设置，确保可见 ===
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

        // 大小：设置稍微大一点，防止看不见
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
    // 1. 特殊天气优先处理 (比如下雨天强制变暗)
    if (currentWeather == WeatherType::Rain) {
        if (overlayLayer) {
            overlayLayer->stopAllActions();
            overlayLayer->setColor(Color3B(30, 40, 60)); // 深灰蓝
            overlayLayer->setOpacity(120);
        }
        return;
    }

    // 2. 获取目标颜色
    Color3B targetColor;
    GLubyte targetOpacity;

    getTargetColor(hour, minute, targetColor, targetOpacity);

    // 3. 应用颜色到图层
    if (overlayLayer) {
        overlayLayer->stopAllActions();
        overlayLayer->setColor(targetColor);
        overlayLayer->setOpacity(targetOpacity);
    }
}

void WeatherManager::getTargetColor(int hour, int minute, Color3B& outColor, GLubyte& outOpacity) {
    int totalMinutes = hour * 60 + minute;
    // 1. 早晨 (6:00 - 7:00): 天色渐亮
    if (totalMinutes >= 6 * 60 && totalMinutes < 7 * 60) {
        float progress = (float)(totalMinutes - 6 * 60) / (1 * 60); // 0.0 -> 1.0
        outColor = Color3B(255, 200, 100);
        outOpacity = static_cast<GLubyte>(MAX_NIGHT_OPACITY * (1.0f - progress));
        return;
    }

    // 2. 白天 (7:00 - 18:00): 白天
    if (totalMinutes >= 7 * 60 && totalMinutes < 18 * 60) {
        outColor = Color3B(0, 0, 0);
        outOpacity = 0;
        return;
    }

    // 3. 黄昏 (18:00 - 20:00): 蓝色变深
    if (totalMinutes >= 18 * 60 && totalMinutes < 20 * 60) {
        float progress = (float)(totalMinutes - 18 * 60) / (2 * 60); 
        GLubyte r = static_cast<GLubyte>(0 + (10 - 0) * progress);
        GLubyte g = static_cast<GLubyte>(50 + (10 - 50) * progress);
        GLubyte b = static_cast<GLubyte>(100 + (30 - 100) * progress);
        outColor = Color3B(r, g, b);
        outOpacity = static_cast<GLubyte>(0 + (MAX_NIGHT_OPACITY * progress));
        return;
    }
    // 4. 夜晚 (20:00 - 次日 6:00)
    if (totalMinutes >= 20 * 60 || totalMinutes < 6 * 60) {
        outColor = Color3B(10, 10, 30);
        outOpacity = MAX_NIGHT_OPACITY;
        return;
    }
}
