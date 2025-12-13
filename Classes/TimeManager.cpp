/****************************************************************
 * Project Name:  StardewValley
 * File Name:     TimeManager.cpp
 ****************************************************************/
#include "TimeManager.h"
#include <string>

USING_NS_CC;

// 定义 Tag
const int TAG_BLACK_MASK = 9999;

#ifndef WINSIZE
#define WINSIZE Director::getInstance()->getVisibleSize()
#endif

TimeManager* TimeManager::instance = nullptr;

TimeManager* TimeManager::getInstance() {
    if (!instance) {
        instance = new TimeManager();
        if (instance && instance->init()) {
            instance->autorelease();
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;
}

TimeManager::TimeManager()
    : accumulatedTime(0)
    , _isInputAllowed(true)    // 默认允许输入
    , _isTransitioning(false)  // 默认未昏迷
    , uiContainer(nullptr)
    , clockBg(nullptr)
    , clockHand(nullptr)
    , timeLabel(nullptr)
    , dateLabel(nullptr)
    , moneyLabel(nullptr)
    , weatherIcon(nullptr)
{
    currentTime = GameTime(1, Season::Spring, 1, 6, 0);
}

TimeManager::~TimeManager() {}

bool TimeManager::init() {
    if (!Node::init()) return false;

    std::srand((unsigned int)time(nullptr));

    currentTime = GameTime(1, Season::Spring, 1, 6, 0);
    WeatherManager::getInstance()->updateWeather(currentTime.season);
    createUI();
    this->scheduleUpdate();
    return true;
}

//  UI 创建
void TimeManager::createUI() {
    uiContainer = Node::create();

    std::string bgPath = "Clock/Clock.png";
    clockBg = Sprite::create(bgPath);

    Size originalSize = Size(300, 100);
    if (clockBg) {
        originalSize = clockBg->getContentSize();
    }

    uiContainer->setContentSize(originalSize);
    uiContainer->setAnchorPoint(Vec2(1, 1));
    uiContainer->setPosition(Vec2(WINSIZE.width, WINSIZE.height + 5));
    uiContainer->setScale(0.24f);

    this->addChild(uiContainer, 100);
    if (clockBg) {
        clockBg->setAnchorPoint(Vec2(0, 0));
        clockBg->setPosition(0, 0);
        uiContainer->addChild(clockBg, 0);
    }

    // 加载指针
    std::string handPath = "Clock/hand.png";
    clockHand = Sprite::create(handPath);

    if (clockHand) {
        clockHand->setAnchorPoint(Vec2(0.5f, 0.0f));
        clockHand->setPosition(Vec2(originalSize.width * 0.33f, originalSize.height * 0.63f));
        clockHand->setScale(0.25f);
        uiContainer->addChild(clockHand, 20);
    }

    // 加载字体
    std::string font = "fonts/arial.ttf";
    if (!FileUtils::getInstance()->isFileExist(font)) font = "Arial";

    // 字号参数
    float fontSizeTime = originalSize.height * 0.10f;
    float fontSizeDate = originalSize.height * 0.10f;
    float fontSizeLabel = originalSize.height * 0.15f;

    // 1.时间 Label
    timeLabel = Label::createWithTTF("12:00 am", font, fontSizeTime);
    if (!timeLabel) 
        timeLabel = Label::createWithSystemFont("12:00 am", "Arial", fontSizeTime);
    timeLabel->setAnchorPoint(Vec2(0.5, 0.5));
    timeLabel->setPosition(Vec2(originalSize.width * 0.72f, originalSize.height * 0.79f));
    timeLabel->setTextColor(Color4B(60, 30, 10, 255));
    uiContainer->addChild(timeLabel, 10);

    // 2. 日期 Label
    dateLabel = Label::createWithTTF("Mon. 1", font, fontSizeDate);
    if (!dateLabel)
        dateLabel = Label::createWithSystemFont("Mon. 1", "Arial", fontSizeDate);
    dateLabel->setAnchorPoint(Vec2(0.5, 0.5));
    dateLabel->setPosition(Vec2(originalSize.width * 0.69f, originalSize.height * 0.47f));
    dateLabel->setTextColor(Color4B(60, 30, 10, 255));
    uiContainer->addChild(dateLabel, 10);

    // 3. 金币 Label
    moneyLabel = Label::createWithTTF("500", font, fontSizeLabel);
    if (!moneyLabel)
        moneyLabel = Label::createWithSystemFont("500", "Arial", fontSizeLabel);
    moneyLabel->setAnchorPoint(Vec2(1, 0.5));
    moneyLabel->setPosition(Vec2(originalSize.width * 0.89f, originalSize.height * 0.19f));
    moneyLabel->setTextColor(Color4B(60, 30, 10, 255));
    uiContainer->addChild(moneyLabel, 10);

    // 4.  天气Label
    weatherIcon = Sprite::create("Clock/sunny.png");
    weatherIcon->setPosition(Vec2(originalSize.width * 0.48f, originalSize.height * 0.64f));
    weatherIcon->setScale(2.0f);
    uiContainer->addChild(weatherIcon, 15);

    refreshUI();
}

//  Update 逻辑 
void TimeManager::update(float dt) {
    if (_isTransitioning) return;

    accumulatedTime += dt;

    // 现实的的0.583秒相当于的游戏里的1分钟
    if (accumulatedTime >= 0.583f) {
        accumulatedTime = 0;

        // 记录旧时间
        GameTime oldTime = currentTime;
        currentTime.addMinutes(1);

        // 触发昏迷
        if (currentTime.hour >= 26) {
            triggerPassOut();
        }

        refreshUI();
    }
}

void TimeManager::triggerPassOut() {
    // 1. 防止重复触发
    if (_isTransitioning) return;
    _isTransitioning = true; 

    // 2. 立刻禁止输入
    _isInputAllowed = false;
    
    // 执行动作序列
    CCLOG("2:00 AM reached. Pausing for 3 seconds...");
}

void TimeManager::startNextDay() {
    CCLOG("Teleporting to next day...");

    // 1. 重置时间
    currentTime.hour = 6;
    currentTime.minute = 0;
    currentTime.addDays(1);

    // 2. 更新天气
    WeatherManager::getInstance()->updateWeather(currentTime.season);

    // 3. 执行传送
    if (onDayStartCallback) {
        onDayStartCallback();
    }

    // 4. 刷新 UI
    refreshUI();
    _isTransitioning = false;
    _isInputAllowed = true;
    accumulatedTime = 0;

    CCLOG("Day %d Started. Player unlocked.", currentTime.dayOfMonth);
}

//  UI 刷新
void TimeManager::refreshUI() {
    // ==================== 1. 时间处理 ====================
    int h = currentTime.hour;
    int m = currentTime.minute;

    // 显示分钟（每10分钟一跳）
    int displayMin = (m / 10) * 10;

    // AM/PM 转换
    std::string ampm;
    int showH = h;

    if (h >= 0 && h <= 12) {
        ampm = "am";
        if (h == 12)
            showH = 12;
    }
    else if (h > 12 && h < 25) {
        ampm = "pm";
        if (h > 12) showH = h - 12; 
    }
    else {
        ampm = "am";
        showH = h - 24; 
    }
    char buf[32];
    sprintf(buf, "%d:%02d%s", showH, displayMin, ampm.c_str());

    if (timeLabel) timeLabel->setString(buf);

    // ==================== 2. 刷新日期 ====================
    if (dateLabel) dateLabel->setString(currentTime.getDateString());

    // ==================== 3. 刷新金币 ====================
    if (moneyLabel) moneyLabel->setString("500");

    // ==================== 4. 指针旋转 ====================
    if (clockHand) {
        // 计算从早上6点到当前时间的分钟数，早上6点 = 指针指向正下方（180度）
        float minutesSince6AM;

        if (h >= 6) {
            // 6:00am之后的时间
            minutesSince6AM = (float)((h - 6) * 60 + m);
        }
        else {
            // 0:00-5:59am的时间（看作前一天的晚上）
            minutesSince6AM = (float)((h + 24 - 6) * 60 + m);
        }

        // 每10分钟一跳，所以用displayMin
        int displayMinutes = (int)(minutesSince6AM / 10) * 10;

        // 从早上6点（180度）开始，每分钟转0.15度，这样20小时（1200分钟）正好转180度
        float rotation = 180.0f + (displayMinutes * 0.15f);

        clockHand->setRotation(rotation);
    }

    // ==================== 5. 刷新天气图标 ====================
    if (weatherIcon) {
        std::string iconPath = "Clock/sunny.png";

        // 获取天气数据
        WeatherType type = WeatherManager::getInstance()->getCurrentWeather();
        Season season = currentTime.season;

        // 判断逻辑
        switch (type) {
        case WeatherType::Sunny:
            iconPath = "Clock/sunny.png";
            break;
        case WeatherType::Rain:
            iconPath = "Clock/rainy.png";
            break;
        case WeatherType::Snow:
            iconPath = "Clock/snowy.png";
            break;
        case WeatherType::Wind:
            if (season == Season::Spring) {
                iconPath = "Clock/windy_spring.png";
            }
            else {
                iconPath = "Clock/windy_autumn.png";
            }
            break;
        default:
            iconPath = "Clock/sunny.png";
            break;
        }

        if (FileUtils::getInstance()->isFileExist(iconPath)) {
            weatherIcon->setTexture(iconPath);
            weatherIcon->setVisible(true);
        }
        else {
            weatherIcon->setVisible(false);
        }
    }

    // ==================== 6. 更新天色 ====================
    // 保留你原本的调用
    WeatherManager::getInstance()->updateSkyColor(h, m);
}

