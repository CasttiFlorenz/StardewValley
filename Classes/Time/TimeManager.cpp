/****************************************************************
 * Project Name:  StardewValley
 * File Name:     TimeManager.cpp
 * File Function: TimeManager类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#include "TimeManager.h"
#include <string>

USING_NS_CC;

// 宏定义：获取屏幕大小
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
// TimeManager.cpp

TimeManager::TimeManager()
    : accumulatedTime(0)
    , uiContainer(nullptr)
    , clockBg(nullptr)
    , clockHand(nullptr)
    , timeLabel(nullptr)
    , dateLabel(nullptr)
    , moneyLabel(nullptr)
    , weatherIcon(nullptr) // <--- 加上这一行初始化
    ,isUpdating(false)
{
    currentTime = GameTime(1, Season::Spring, 1, 6, 0);
}

TimeManager::~TimeManager() {}

bool TimeManager::init() {
    if (!Node::init()) return false;

    // 1. 创建 UI
    createUI();
    return true;
}

void TimeManager::createUI() {
    // 创建容器
    uiContainer = Node::create();

    // 1. 加载背景
    std::string bgPath = "Clock/Clock.png";
    clockBg = Sprite::create(bgPath);

    //以此获取原始尺寸作为基准
    Size originalSize = Size(300, 100);
    if (clockBg) {
        originalSize = clockBg->getContentSize();
    }

    uiContainer->setContentSize(originalSize);

    // 设置锚点为右上角 (1, 1)，位置在屏幕右上角
    uiContainer->setAnchorPoint(Vec2(1, 1));
    // 参数1：Y轴 + 5
    uiContainer->setPosition(Vec2(WINSIZE.width, WINSIZE.height + 5));

    // 参数2：整体缩放 0.12
    uiContainer->setScale(0.24f);

    this->addChild(uiContainer, 100);

    // 添加背景图
    if (clockBg) {
        clockBg->setAnchorPoint(Vec2(0, 0));
        clockBg->setPosition(0, 0);
        uiContainer->addChild(clockBg, 0);
    }

    // 2. 加载指针
    std::string handPath = "Clock/hand.png";
    clockHand = Sprite::create(handPath);

    if (clockHand) {
        // 参数1：锚点
        clockHand->setAnchorPoint(Vec2(0.5f, 0.0f));

        // 参数2：位置
        clockHand->setPosition(Vec2(originalSize.width * 0.33f, originalSize.height * 0.63f));

        // 参数3：指针单独缩放 
        clockHand->setScale(0.25f);

        // 参数4：层级20
        uiContainer->addChild(clockHand, 20); 
    }

    // 3. 加载字体
    std::string font = "fonts/arial.ttf";
    if (!FileUtils::getInstance()->isFileExist(font)) font = "Arial";

    // 字号参数
    float fontSizeTime = originalSize.height * 0.10f;
    float fontSizeDate = originalSize.height * 0.10f;
    float fontSizeLabel = originalSize.height * 0.15f;

    // 4. 时间 Label
    timeLabel = Label::createWithTTF("12:00 am", font, fontSizeTime);
    if (!timeLabel) timeLabel = Label::createWithSystemFont("12:00 am", "Arial", fontSizeTime);

    timeLabel->setAnchorPoint(Vec2(0.5, 0.5));
    // 位置参数
    timeLabel->setPosition(Vec2(originalSize.width * 0.72f, originalSize.height * 0.79f));
    timeLabel->setTextColor(Color4B(60, 30, 10, 255));
    uiContainer->addChild(timeLabel, 10);

    // 5. 日期 Label
    dateLabel = Label::createWithTTF("Mon. 1", font, fontSizeDate);
    if (!dateLabel) dateLabel = Label::createWithSystemFont("Mon. 1", "Arial", fontSizeDate);

    dateLabel->setAnchorPoint(Vec2(0.5, 0.5));
    // 位置参数
    dateLabel->setPosition(Vec2(originalSize.width * 0.69f, originalSize.height * 0.47f));
    dateLabel->setTextColor(Color4B(60, 30, 10, 255));
    uiContainer->addChild(dateLabel, 10);

    // 6. 金币 Label
    moneyLabel = Label::createWithTTF("500", font, fontSizeLabel);
    if (!moneyLabel) moneyLabel = Label::createWithSystemFont("500", "Arial", fontSizeLabel);

    moneyLabel->setAnchorPoint(Vec2(1, 0.5));
    // 位置参数
    moneyLabel->setPosition(Vec2(originalSize.width * 0.89f, originalSize.height * 0.19f));
    moneyLabel->setTextColor(Color4B(60, 30, 10, 255));
    uiContainer->addChild(moneyLabel, 10);

   // 7.  天气Label
   // 假设初始是晴天
    weatherIcon = Sprite::create("Clock/sunny.png");
  
   
    weatherIcon->setPosition(Vec2(originalSize.width * 0.48f, originalSize.height * 0.64f));

    // 缩放图标 (如果图标太大)
    weatherIcon->setScale(2.0f);

    uiContainer->addChild(weatherIcon, 15); // 层级比文字高一点

    // 初始刷新
    refreshUI();
}void TimeManager::update(float dt) {
    // 0.07秒现实时间 = 1分钟游戏时间
    accumulatedTime += dt;
    // 使用您的计时速率
    if (accumulatedTime >= 0.00007f) {
        accumulatedTime = 0;

        // 记录旧时间，用于判断是否发生过夜
        GameTime oldTime = currentTime;

        // 1. 先增加1分钟 (GameTime内部会自动处理小时进位)
        currentTime.addMinutes(1);

        // 2. 检查是否超过 2:00 am (hour >= 26)
        if (oldTime.hour < 26 && currentTime.hour >= 26) {
            // 触发过夜逻辑
         currentTime.addDays(1);

            // 重设时间到 6:00 am
            currentTime.hour = 6;
            currentTime.minute = 0;

            // 检查是否发生了日期/季节变化
            if (!oldTime.isSameDay(currentTime)) {
                WeatherManager::getInstance()->updateWeather(currentTime.season);
            }
        }

        refreshUI();
    }
}
    void TimeManager::refreshUI() {
    // ==================== 1. 时间处理 ====================
    int h = currentTime.hour;
    int m = currentTime.minute;

    // 显示分钟（每10分钟一跳）
    int displayMin = (m / 10) * 10;

    // AM/PM 转换
    std::string ampm;
    int showH = h;

    if (h >= 0 && h < 12) {
        ampm = "am";
        if (h == 0) showH = 12; // 0点显示为12am
    }
    else if(h>12&&h<25){
        ampm = "pm";
        if (h > 12) showH = h - 12; // 13-23点转换为1-11pm
        if (h == 12) showH = 12;    // 12点显示为12pm
    }
    else {
        ampm = "am";
        showH = h - 24; // 13-23点转换为1-11pm
    }

    // 格式化时间字符串
    char buf[32];
   
        sprintf(buf, "%d:%02d%s", showH, displayMin, ampm.c_str()); 
 

    if (timeLabel) timeLabel->setString(buf);

    // ==================== 2. 刷新日期 ====================
    if (dateLabel) dateLabel->setString(currentTime.getDateString());

    // ==================== 3. 刷新金币 ====================
    if (moneyLabel) moneyLabel->setString("500");

    // ==================== 4. 指针旋转 ====================
    if (clockHand) {
        // 计算从早上6点到当前时间的分钟数
        // 早上6点 = 指针指向正下方（180度）
        float minutesSince6AM;

        if (h >= 6) {
            // 6:00am之后的时间
            minutesSince6AM = (h - 6) * 60 + m;
        }
        else {
            // 0:00-5:59am的时间（看作前一天的晚上）
            minutesSince6AM = (h + 24 - 6) * 60 + m;
        }

        // 每10分钟一跳，所以用displayMin
        int displayMinutes = (minutesSince6AM / 10) * 10;

        // 从早上6点（180度）开始，每分钟转0.15度
        // 这样20小时（1200分钟）正好转180度
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

        // 应用纹理
        if (FileUtils::getInstance()->isFileExist(iconPath)) {
            weatherIcon->setTexture(iconPath);
            weatherIcon->setVisible(true);
        }
        else {
            weatherIcon->setVisible(false);
        }
    }

    // ==================== 6. 更新天色 ====================
    WeatherManager::getInstance()->updateSkyColor(h, m);
}
