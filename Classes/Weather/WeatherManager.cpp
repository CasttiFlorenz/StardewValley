/****************************************************************
 * Project Name:  StardewValley
 * File Name:     WeatherManager.cpp
 * File Function: WeatherManager类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#include "WeatherManager.h"
#include <new> 

USING_NS_CC;

// 静态成员变量初始化
WeatherManager* WeatherManager::_instance = nullptr;

/**
 * 获取 WeatherManager 单例对象
 * 使用 std::nothrow 分配内存，初始化后调用 retain() 增加引用计数以防止被 Cocos2d-x 自动释放池清理
 * @return WeatherManager 的唯一实例指针
 */
WeatherManager* WeatherManager::getInstance() {
    // 检查实例是否为空，确保单例唯一性
    if (_instance == nullptr) {
        // 使用 std::nothrow 分配内存，失败返回 nullptr 而不抛出异常
        _instance = new (std::nothrow) WeatherManager();

        // 实例创建成功后进行初始化
        if (_instance && _instance->init()) {
            // 将对象加入自动释放池
            _instance->autorelease();
            // 增加引用计数，防止被自动释放池清理，确保单例存活
            _instance->retain();
        }
        else {
            // 初始化失败，安全删除并置空
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}


/**
 * 销毁 WeatherManager 单例
 * 释放实例引用计数并置空指针，清理内存
 */
void WeatherManager::destroyInstance() {
    // 检查实例有效性
    if (_instance != nullptr) {
        // 减少引用计数，触发析构函数
        _instance->release();
        // 指针置空，避免悬空指针
        _instance = nullptr;
    }
}



/**
 * 构造函数
 * 初始化成员变量为默认值，具体资源创建推迟到 init() 中执行
 */
WeatherManager::WeatherManager()
    : _currentWeather(WeatherType::Sunny)
    , _currentSeason(Season::Spring)
    , _overlayLayer(nullptr)
    , _weatherLayer(nullptr)
    , _particleEffect(nullptr) {
    // 构造函数仅初始化成员变量，具体逻辑在 init 中处理
}



/**
 * 析构函数
 * Cocos2d-x 的 Node 析构机制会自动清理子节点（overlayLayer 和 weatherLayer），无需手动 delete
 */
WeatherManager::~WeatherManager() {
    // 析构函数，Cocos2d-x 节点会自动清理子节点内存
}



/**
 * 初始化管理器
 * 创建全屏颜色覆盖层（用于光照/昼夜更替）和天气粒子层，并设置初始层级和透明度
 * @return 初始化成功返回 true，否则返回 false
 */
bool WeatherManager::init() {
    // 直接调用引擎接口获取屏幕尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 初始化滤镜层 (全屏颜色层)
    _overlayLayer = LayerColor::create(Color4B(0, 0, 0, 0), visibleSize.width, visibleSize.height);

    // 空指针检查
    if (_overlayLayer == nullptr) {
        return false;
    }

    // 启用级联透明度，确保子节点随父节点透明度变化
    _overlayLayer->setCascadeOpacityEnabled(true);
    // 设置高 ZOrder (9999) 确保覆盖在游戏场景最上层
    this->addChild(_overlayLayer, 9999);

    // 2. 初始化天气粒子层
    _weatherLayer = Layer::create();
    // 空指针检查
    if (_weatherLayer == nullptr) {
        return false;
    }
    this->addChild(_weatherLayer, 1);

    // 3. 设置初始状态
    _overlayLayer->setVisible(true);
    _overlayLayer->setColor(Color3B::BLACK);
    _overlayLayer->setOpacity(0);

    return true;
}



/**
 * 根据季节更新天气
 * 基于随机概率（定义在 Constant.h）决定当天的天气（晴/雨/风/雪），并调用 setVisuals 更新视觉表现
 * @param season 当前游戏季节
 */
void WeatherManager::updateWeather(Season season) {
    _currentSeason = season;

    // 生成 0-99 的随机数用于概率判断
    int chance = rand() % 100;

    // 根据季节与随机数确定天气类型
    // 概率阈值定义在 Constant.h 中
    switch (season) {
    case Season::Spring:
        if (chance < CHANCE_RAIN_LOW) {
            _currentWeather = WeatherType::Rain;
        }
        else if (chance < CHANCE_WIND_LOW) {
            _currentWeather = WeatherType::Wind;
        }
        else {
            _currentWeather = WeatherType::Sunny;
        }
        break;

    case Season::Fall:
        if (chance < CHANCE_RAIN_LOW) {
            _currentWeather = WeatherType::Rain;
        }
        else if (chance < CHANCE_WIND_FALL) {
            _currentWeather = WeatherType::Wind;
        }
        else {
            _currentWeather = WeatherType::Sunny;
        }
        break;

    case Season::Summer:
        if (chance < CHANCE_RAIN_LOW) {
            _currentWeather = WeatherType::Rain;
        }
        else {
            _currentWeather = WeatherType::Sunny;
        }
        break;

    case Season::Winter:
        if (chance < CHANCE_SNOW) {
            _currentWeather = WeatherType::Snow;
        }
        else {
            _currentWeather = WeatherType::Sunny;
        }
        break;

    default:
        _currentWeather = WeatherType::Sunny;
        break;
    }

    // 更新视觉表现
    setVisuals(_currentWeather);
}



/**
 * 强制设置天气类型
 * 直接改变当前天气状态并立即应用对应的视觉效果（用于调试或剧情强制触发）
 * @param type 目标天气类型
 */
void WeatherManager::setWeather(WeatherType type) {
    _currentWeather = type;
    setVisuals(_currentWeather);
}



/**
 * 检查当前是否在下雨
 * @return 如果是雨天返回 true，否则返回 false
 */
bool WeatherManager::isRaining() const {
    return _currentWeather == WeatherType::Rain;
}



/**
 * 获取当前天气类型
 * @return 当前的 WeatherType 枚举值
 */
WeatherType WeatherManager::getCurrentWeather() const {
    return _currentWeather;
}

 /**
  * 辅助函数：安全获取纹理
  * 尝试加载指定路径的图片；如果失败，则动态生成一个纯白色的 8x8 纹理作为保底，防止程序崩溃
  * @param filename 图片资源路径
  * @return 有效的 Texture2D 指针
  */

Texture2D* WeatherManager::getTextureOrFallback(const std::string& filename) {
    auto textureCache = Director::getInstance()->getTextureCache();
    // 引擎组件判空
    if (textureCache == nullptr) {
        return nullptr;
    }

    // 1. 尝试从文件加载纹理
    auto texture = textureCache->addImage(filename);
    if (texture != nullptr) {
        return texture;
    }

    // 2. 检查缓存中是否已有保底纹理
    texture = textureCache->getTextureForKey(PATH_IMG_FALLBACK_KEY);
    if (texture != nullptr) {
        return texture;
    }

    // 3. 动态生成纯白保底纹理
    const int w = FALLBACK_TEXTURE_SIZE;
    const int h = FALLBACK_TEXTURE_SIZE;
    const int dataSize = w * h * 4;

    // 申请像素数据内存
    unsigned char* data = new (std::nothrow) unsigned char[dataSize];
    if (data == nullptr) {
        return nullptr;
    }

    // 填充白色 (RGBA: 255)
    for (int i = 0; i < dataSize; i++) {
        data[i] = 255;
    }

    // 创建 Image 对象处理数据
    Image* image = new (std::nothrow) Image();
    if (image != nullptr) {
        image->initWithRawData(data, dataSize, w, h, 8);
        texture = textureCache->addImage(image, PATH_IMG_FALLBACK_KEY);
        // 释放 Image 内存
        image->release();
    }

    // 释放原始数据内存
    delete[] data;
    return texture;
}

 /**
  * 设置天气的视觉效果（粒子系统）
  * 清理旧粒子，根据天气类型创建新的粒子系统（如风中的花瓣、雨滴、雪花），并配置物理属性（重力、速度、生命周期）
  * @param type 天气类型
  */

void WeatherManager::setVisuals(WeatherType type) {
    // 移除旧的粒子特效
    if (_particleEffect != nullptr) {
        _particleEffect->removeFromParent();
        _particleEffect = nullptr;
    }

    // 容器判空
    if (_weatherLayer == nullptr) {
        return;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    switch (type) {
    case WeatherType::Sunny:
        // 晴天无特效
        break;

    case WeatherType::Wind:
    {
        // 根据季节选择花瓣或落叶纹理
        std::string textureFile = (_currentSeason == Season::Spring) ? PATH_IMG_FLOWER : PATH_IMG_LEAF;
        auto texture = getTextureOrFallback(textureFile);

        // 纹理加载失败保护
        if (texture == nullptr) return;

        _particleEffect = ParticleSystemQuad::createWithTotalParticles(200);
        if (_particleEffect == nullptr) return;

        // 设置粒子属性
        _particleEffect->setTexture(texture);
        _particleEffect->setLife(PARTICLE_LIFE_DEFAULT);
        _particleEffect->setLifeVar(PARTICLE_LIFE_VAR);

        // 计算发射速率
        float life = _particleEffect->getLife();
        if (life > 0) {
            _particleEffect->setEmissionRate(_particleEffect->getTotalParticles() / life);
        }

        _particleEffect->setDuration(ParticleSystem::DURATION_INFINITY);
        _particleEffect->setEmitterMode(ParticleSystem::Mode::GRAVITY);

        // 设置位置和物理参数 (模拟风吹)
        _particleEffect->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + 50));
        _particleEffect->setPosVar(Vec2(visibleSize.width, 0));
        _particleEffect->setGravity(Vec2(200, -100));
        _particleEffect->setSpeed(150);
        _particleEffect->setSpeedVar(50);
        _particleEffect->setAngle(-45);
        _particleEffect->setAngleVar(20);
        _particleEffect->setStartSize(25.0f);
        _particleEffect->setStartSizeVar(10.0f);
        _particleEffect->setEndSize(15.0f);

        // 设置颜色
        if (_currentSeason == Season::Spring) {
            _particleEffect->setStartColor(Color4F(1.0f, 0.7f, 0.8f, 1.0f));
            _particleEffect->setEndColor(Color4F(1.0f, 0.7f, 0.8f, 0.5f));
        }
        else {
            _particleEffect->setStartColor(Color4F(1.0f, 0.6f, 0.2f, 1.0f));
            _particleEffect->setEndColor(Color4F(0.8f, 0.5f, 0.1f, 0.5f));
        }

        _weatherLayer->addChild(_particleEffect);
        break;
    }

    case WeatherType::Rain:
    {
        _particleEffect = ParticleSystemQuad::createWithTotalParticles(1500);
        if (_particleEffect == nullptr) return;

        auto texture = Director::getInstance()->getTextureCache()->addImage(PATH_IMG_RAINDROP);
        if (texture != nullptr) {
            _particleEffect->setTexture(texture);
        }

        // 设置下雨粒子参数
        _particleEffect->setDuration(ParticleSystem::DURATION_INFINITY);
        _particleEffect->setEmitterMode(ParticleSystem::Mode::GRAVITY);
        _particleEffect->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + 100));
        _particleEffect->setPosVar(Vec2(visibleSize.width / 2, 20));
        _particleEffect->setGravity(Vec2(40, -1000));
        _particleEffect->setSpeed(50);
        _particleEffect->setSpeedVar(20);
        _particleEffect->setEmissionRate(100.0f);
        _particleEffect->setLife(RAIN_LIFE);
        _particleEffect->setLifeVar(RAIN_LIFE_VAR);
        _particleEffect->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 0.9f));
        _particleEffect->setEndColor(Color4F(0.85f, 0.9f, 1.0f, 0.7f));
        _particleEffect->setStartSize(20.0f);
        _particleEffect->setStartSizeVar(5.0f);
        _particleEffect->setEndSize(15.0f);
        _particleEffect->setAngle(270);
        _particleEffect->setAngleVar(15);
        _particleEffect->setBlendAdditive(true);

        _weatherLayer->addChild(_particleEffect);
        break;
    }
    case WeatherType::Snow:
    {
        // 使用引擎内置雪效
        _particleEffect = ParticleSnow::create();
        if (_particleEffect == nullptr) return;

        _particleEffect->setGravity(Vec2(0, -3));
        _particleEffect->setSpeed(20.0f);
        _particleEffect->setSpeedVar(3.0f);
        _particleEffect->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));

        _weatherLayer->addChild(_particleEffect);
        break;
    }
    }
}



 /**
  * 更新天色与光照
  * 根据游戏时间计算覆盖层的颜色和透明度；如果是雨天，会忽略时间强制使天色变暗
  * @param hour   当前小时
  * @param minute 当前分钟
  */
void WeatherManager::updateSkyColor(int hour, int minute) {
    // 雨天强制变暗
    if (_currentWeather == WeatherType::Rain) {
        if (_overlayLayer != nullptr) {
            _overlayLayer->stopAllActions();
            _overlayLayer->setColor(Color3B(30, 40, 60));
            _overlayLayer->setOpacity(60);
        }
        return;
    }

    Color3B targetColor;
    GLubyte targetOpacity;

    // 计算目标光照
    getTargetColor(hour, minute, targetColor, targetOpacity);

    // 应用颜色和透明度
    if (_overlayLayer != nullptr) {
        _overlayLayer->stopAllActions();
        _overlayLayer->setColor(targetColor);
        _overlayLayer->setOpacity(targetOpacity);
    }
}




 /**
  * 计算目标光照颜色
  * 根据具体的时间点（总分钟数）计算早晨、白天、黄昏、夜晚的颜色渐变值
  * @param hour       当前小时
  * @param minute     当前分钟
  * @param outColor   [输出] 计算得到的 RGB 颜色
  * @param outOpacity [输出] 计算得到的透明度值
  */
void WeatherManager::getTargetColor(int hour, int minute, Color3B& outColor, GLubyte& outOpacity) {
    int totalMinutes = hour * 60 + minute;

    // 1. 早晨 (6:00 - 7:00): 天色渐亮
    if (totalMinutes >= 6 * 60 && totalMinutes < 7 * 60) {
        float progress = static_cast<float>(totalMinutes - 6 * 60) / 60.0f;
        outOpacity = static_cast<GLubyte>(MAX_NIGHT_OPACITY * 0.2f * (1.0f - progress));
        return;
    }

    // 2. 白天 (7:00 - 18:00): 完全明亮
    if (totalMinutes >= 7 * 60 && totalMinutes < 18 * 60) {
        outColor = Color3B(0, 0, 0);
        outOpacity = 0;
        return;
    }

    // 3. 黄昏 (18:00 - 20:00): 渐变到夜色
    if (totalMinutes >= 18 * 60 && totalMinutes < 20 * 60) {
        float progress = static_cast<float>(totalMinutes - 18 * 60) / 120.0f;

        GLubyte r = static_cast<GLubyte>(0 + (10 - 0) * progress);
        GLubyte g = static_cast<GLubyte>(50 + (10 - 50) * progress);
        GLubyte b = static_cast<GLubyte>(100 + (30 - 100) * progress);

        outColor = Color3B(r, g, b);
        outOpacity = static_cast<GLubyte>(0 + (MAX_NIGHT_OPACITY * progress));
        return;
    }

    // 4. 夜晚 (20:00 - 次日 6:00): 维持黑暗
    if (totalMinutes >= 20 * 60 || totalMinutes < 6 * 60) {
        outColor = Color3B(10, 10, 30);
        outOpacity = static_cast<GLubyte>(MAX_NIGHT_OPACITY);
        return;
    }
}
