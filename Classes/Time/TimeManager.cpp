/****************************************************************
 * Project Name : StardewValley
 * File Name : TimeManager.cpp
 * File Function : TimeManager类的实现
 * Author : 赵睿妍
 * Update Date : 2025 / 12 / 13
 * License : MIT License
 ****************************************************************/

#include "TimeManager.h"
#include <string>
#include <new> 

USING_NS_CC;

// 静态成员初始化
TimeManager* TimeManager::_instance = nullptr;

/**
 * 获取 TimeManager 单例对象，如果不存在则创建并进行初始化
 * @return TimeManager 的唯一实例指针
 */
TimeManager* TimeManager::getInstance() {
    // 检查实例是否存在
    if (!_instance) {
        // 使用 std::nothrow 避免内存分配失败抛出异常
        _instance = new (std::nothrow) TimeManager();
        if (_instance && _instance->init()) {
            _instance->autorelease();
            _instance->retain(); // 保持引用，防止被自动释放
        }
        else {
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

/**
 * 销毁单例对象并释放相关内存
 */
void TimeManager::destroyInstance() {
    // 销毁单例，释放内存
    if (_instance != nullptr) {
        _instance->release();
        _instance = nullptr;
    }
}

/**
 * 构造函数，初始化成员变量和默认游戏时间
 */
TimeManager::TimeManager()
    : _accumulatedTime(0.0f)
    , _uiContainer(nullptr)
    , _isInputAllowed(true)
    , _isTransitioning(false)
    , _isUpdating(false)
    , _clockBg(nullptr)
    , _clockHand(nullptr)
    , _timeLabel(nullptr)
    , _dateLabel(nullptr)
    , _moneyLabel(nullptr)
    , _weatherIcon(nullptr)
{
    // 构造函数初始化
    _currentTime = GameTime(1, Season::Spring, 1, 6, 0);
}

/**
 * 析构函数，Cocos2d-x 会自动清理 Node 资源
 */
TimeManager::~TimeManager() {
    // 析构逻辑
}

/**
 * 初始化管理器，设置随机种子、天气并创建 UI
 * @return 初始化成功返回 true
 */
bool TimeManager::init() {
    if (!Node::init()) return false;

    // 设置随机数种子
    std::srand(static_cast<unsigned int>(time(nullptr)));

    _isUpdating = false;
    _currentTime = GameTime(1, Season::Spring, 1, 6, 0);

    // 初始化天气
    if (WeatherManager::getInstance() != nullptr) {
        WeatherManager::getInstance()->updateWeather(_currentTime.getSeason());
    }

    createUI();
    return true;
}

/**
 * 切换每帧更新状态（暂停/恢复时间流动）
 */
void TimeManager::changeUpdateStatus() {
    // 切换更新状态
    if (_isUpdating) {
        this->unscheduleUpdate();
    }
    else {
        this->scheduleUpdate();
    }
    _isUpdating = !_isUpdating;
}

/**
 * 创建时间显示 UI，包括时钟、日期、金钱和天气图标
 */
void TimeManager::createUI() {
    // 直接调用 API 获取屏幕尺寸，不使用宏
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _uiContainer = Node::create();
    if (_uiContainer == nullptr) return;

    // 创建时钟背景
    _clockBg = Sprite::create(PATH_UI_CLOCK_BG);
    Size originalSize = Size(300, 100);

    if (_clockBg != nullptr) {
        originalSize = _clockBg->getContentSize();
        _clockBg->setAnchorPoint(Vec2(0, 0));
        _clockBg->setPosition(0, 0);
        _uiContainer->addChild(_clockBg, 0);
    }

    _uiContainer->setContentSize(originalSize);
    _uiContainer->setAnchorPoint(Vec2(1, 1));
    // 使用 visibleSize 替代 WINSIZE
    _uiContainer->setPosition(Vec2(visibleSize.width, visibleSize.height + 5));
    _uiContainer->setScale(0.24f);

    this->addChild(_uiContainer, ZORDER_UI_CONTAINER);

    // 加载指针
    _clockHand = Sprite::create(PATH_UI_CLOCK_HAND);
    if (_clockHand != nullptr) {
        _clockHand->setAnchorPoint(Vec2(0.5f, 0.0f));
        _clockHand->setPosition(Vec2(originalSize.width * 0.33f, originalSize.height * 0.63f));
        _clockHand->setScale(0.25f);
        _uiContainer->addChild(_clockHand, ZORDER_UI_HAND);
    }

    // 加载字体
    std::string font = PATH_FONT_ARIAL;
    if (!FileUtils::getInstance()->isFileExist(font)) {
        font = FONT_DEFAULT_SYSTEM;
    }

    // 字号参数
    float fontSizeTime = originalSize.height * 0.10f;
    float fontSizeDate = originalSize.height * 0.10f;
    float fontSizeLabel = originalSize.height * 0.15f;
    Color4B textColor(60, 30, 10, 255);

    // 1. 时间 Label
    _timeLabel = Label::createWithTTF("12:00 am", font, fontSizeTime);
    if (!_timeLabel) {
        _timeLabel = Label::createWithSystemFont("12:00 am", FONT_DEFAULT_SYSTEM, fontSizeTime);
    }
    if (_timeLabel) {
        _timeLabel->setAnchorPoint(Vec2(0.5, 0.5));
        _timeLabel->setPosition(Vec2(originalSize.width * 0.72f, originalSize.height * 0.79f));
        _timeLabel->setTextColor(textColor);
        _uiContainer->addChild(_timeLabel, ZORDER_UI_LABEL);
    }

    // 2. 日期 Label
    _dateLabel = Label::createWithTTF("Mon. 1", font, fontSizeDate);
    if (!_dateLabel) {
        _dateLabel = Label::createWithSystemFont("Mon. 1", FONT_DEFAULT_SYSTEM, fontSizeDate);
    }
    if (_dateLabel) {
        _dateLabel->setAnchorPoint(Vec2(0.5, 0.5));
        _dateLabel->setPosition(Vec2(originalSize.width * 0.69f, originalSize.height * 0.47f));
        _dateLabel->setTextColor(textColor);
        _uiContainer->addChild(_dateLabel, ZORDER_UI_LABEL);
    }

    // 3. 金币 Label
    _moneyLabel = Label::createWithTTF("500", font, fontSizeLabel);
    if (!_moneyLabel) {
        _moneyLabel = Label::createWithSystemFont("500", FONT_DEFAULT_SYSTEM, fontSizeLabel);
    }
    if (_moneyLabel) {
        _moneyLabel->setAnchorPoint(Vec2(1, 0.5));
        _moneyLabel->setPosition(Vec2(originalSize.width * 0.89f, originalSize.height * 0.19f));
        _moneyLabel->setTextColor(textColor);
        _uiContainer->addChild(_moneyLabel, ZORDER_UI_LABEL);
    }

    // 4. 天气图标
    _weatherIcon = Sprite::create(PATH_UI_WEATHER_SUNNY);
    if (_weatherIcon) {
        _weatherIcon->setPosition(Vec2(originalSize.width * 0.48f, originalSize.height * 0.64f));
        _weatherIcon->setScale(2.0f);
        _uiContainer->addChild(_weatherIcon, ZORDER_UI_WEATHER);
    }

    refreshUI();
}

/**
 * 设定时间
 */
void TimeManager::setTime(const GameTime& time)
{
    _currentTime = time;
};

/**
 * 每帧更新时间逻辑，处理分钟增加和强制昏迷检测
 * @param dt 帧间隔时间
 */
void TimeManager::update(float dt) {
    if (_isTransitioning) return; // 如果正在转场，暂停时间

    _accumulatedTime += dt;

    // 使用常量 TIME_UPDATE_INTERVAL (0.7f)
    if (_accumulatedTime >= TIME_UPDATE_INTERVAL) {
        _accumulatedTime = 0;

        _currentTime.addMinutes(1);

        // 检查是不是到了强制昏迷时间
        if (_currentTime.getHour() >= TIME_PASS_OUT_HOUR) {
            triggerPassOut();
        }
        refreshUI();

        // 重置累积时间
        _accumulatedTime = 0;
    }
}

/**
 * 触发凌晨2点强制昏迷逻辑，禁用输入并开始下一天转场
 */
void TimeManager::triggerPassOut() {
    // 防止重复触发
    if (_isTransitioning) return;

    // 标记开始转场，禁止输入
    _isTransitioning = true;
    _isInputAllowed = false;

    CCLOG("2:00 AM reached. Pausing...");

    auto callback = CallFunc::create([this]() {
        this->startNextDay();
        });
    auto delay = DelayTime::create(TIME_PASS_OUT_DELAY);

    this->runAction(Sequence::create(delay, callback, nullptr));
}

/**
 * 开始新的一天，重置时间、更新天气并恢复输入
 */
void TimeManager::startNextDay() {
    CCLOG("Teleporting to next day...");

    _currentTime.addDays(1);
    // 重置时间为早上 6:00
    _currentTime = GameTime(_currentTime.getYear(), _currentTime.getSeason(), _currentTime.getDayOfMonth(), 6, 0);

    if (WeatherManager::getInstance() != nullptr) {
        WeatherManager::getInstance()->updateWeather(_currentTime.getSeason());
    }

    // 执行回调
    if (onDayStartCallback) {
        onDayStartCallback();
    }

    refreshUI();
    _isTransitioning = false;
    _isInputAllowed = true;

    CCLOG("Day %d Started. Player unlocked.", _currentTime.getDayOfMonth());
}

/**
 * 刷新 UI 显示，包括时间文本、日期、金钱、指针旋转角度和天气图标
 */
void TimeManager::refreshUI() {
    // ==================== 1. 时间处理 ====================
    int h = _currentTime.getHour();
    int m = _currentTime.getMinute();

    // 格式化时间显示
    if (_timeLabel != nullptr) {
        // GameTime 类已封装好格式化逻辑，这里可以简化，但为了保持您原始逻辑的显示效果，
        // 这里沿用 GameTime 的字符串获取
        _timeLabel->setString(_currentTime.getTimeString());
    }

    // ==================== 2. 刷新日期 ====================
    if (_dateLabel != nullptr) {
        _dateLabel->setString(_currentTime.getDateString());
    }

    // ==================== 3. 刷新金币 ====================
    if (_moneyLabel != nullptr) {
        // 使用 Money 单例获取数据
        if (Money::getInstance() != nullptr) {
            int currentMoney = Money::getInstance()->getMoney();
            _moneyLabel->setString(StringUtils::format("%d", currentMoney));
        }
    }

    // ==================== 4. 指针旋转 ====================
    if (_clockHand != nullptr) {
        float minutesSince6AM;

        if (h >= 6) {
            minutesSince6AM = static_cast<float>((h - 6) * 60 + m);
        }
        else {
            minutesSince6AM = static_cast<float>((h + 24 - 6) * 60 + m);
        }

        // 计算显示角度
        int displayMinutes = (static_cast<int>(minutesSince6AM) / 10) * 10;
        float rotation = ROTATION_BASE_ANGLE + (static_cast<float>(displayMinutes) * ROTATION_PER_MINUTE);

        _clockHand->setRotation(rotation);
    }

    // ==================== 5. 刷新天气图标 ====================
    if (_weatherIcon != nullptr && WeatherManager::getInstance() != nullptr) {
        std::string iconPath = PATH_UI_WEATHER_SUNNY;

        WeatherType type = WeatherManager::getInstance()->getCurrentWeather();
        Season season = _currentTime.getSeason();

        switch (type) {
        case WeatherType::Sunny:
            iconPath = PATH_UI_WEATHER_SUNNY;
            break;
        case WeatherType::Rain:
            iconPath = PATH_UI_WEATHER_RAINY;
            break;
        case WeatherType::Snow:
            iconPath = PATH_UI_WEATHER_SNOWY;
            break;
        case WeatherType::Wind:
            if (season == Season::Spring) {
                iconPath = PATH_UI_WEATHER_WINDY_SPRING;
            }
            else {
                iconPath = PATH_UI_WEATHER_WINDY_AUTUMN;
            }
            break;
        default:
            iconPath = PATH_UI_WEATHER_SUNNY;
            break;
        }

        // 检查文件是否存在并设置纹理
        if (FileUtils::getInstance()->isFileExist(iconPath)) {
            _weatherIcon->setTexture(iconPath);
            _weatherIcon->setVisible(true);
        }
        else {
            _weatherIcon->setVisible(false);
        }
    }

    // ==================== 6. 更新天色 ====================
    if (WeatherManager::getInstance() != nullptr) {
        WeatherManager::getInstance()->updateSkyColor(h, m);
    }
}

/**
 * 启动睡觉流程，显示遮罩动画并在结束后开始新的一天
 */
void TimeManager::startSleepSequence() {
    if (_isTransitioning) return;

    _isTransitioning = true;
    _isInputAllowed = false;

    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene == nullptr) return;

    // 直接获取尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto sleepBg = Sprite::create(PATH_UI_SLEEP_BG);

    if (sleepBg == nullptr) return;

    sleepBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    sleepBg->setScale(0.75f);
    sleepBg->setOpacity(100);
    sleepBg->setCascadeOpacityEnabled(true);
    runningScene->addChild(sleepBg, ZORDER_SLEEP_MASK);

    auto label = Label::createWithSystemFont("Sleeping...", PATH_FONT_PIXEL, 100);
    if (label != nullptr) {
        label->setTextColor(Color4B::BLACK);
        Size bgSize = sleepBg->getContentSize();
        label->setPosition(Vec2(bgSize.width / 2, bgSize.height / 2));
        sleepBg->setTag(SLEEPING_TAG);
        sleepBg->addChild(label);
    }

    // 动画序列：淡入 -> 等待 -> 下一天 -> 淡出 -> 清理
    auto seq = Sequence::create(
        FadeIn::create(0.5f),
        DelayTime::create(3.0f),
        CallFunc::create([this]() {
            this->startNextDay();
            }),
        FadeOut::create(1.0f),
        CallFunc::create([sleepBg, this]() {
            sleepBg->removeFromParent();
            this->_isTransitioning = false;
            this->_isInputAllowed = true;
            }),
        nullptr
    );

    sleepBg->runAction(seq);

}
