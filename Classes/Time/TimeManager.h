/****************************************************************
 * Project Name:  StardewValley
 * File Name:     TimeManager.h
 * File Function: TimeManager类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "cocos2d.h"
#include "GameTime.h"
#include "../Weather/WeatherManager.h"
#include <functional> 
#include"../Money/Money.h"
class TimeManager : public cocos2d::Node {
public:
    static TimeManager* getInstance();
    virtual bool init(); // 简化初始化，参数写死在内部
    void update(float dt) override; // 必须有update才能走时间

    // 获取当前时间数据
    GameTime getCurrentTime() const { return currentTime; }
    void changeUpdateStatus()noexcept { if (isUpdating)unscheduleUpdate(); else scheduleUpdate(); isUpdating = !isUpdating; };
    void startNextDay();
    std::function<void()> onDayStartCallback;
    void triggerPassOut();
private:
    bool _isTransitioning; // 是否正在转场
    bool _isInputAllowed;  // 是否允许输入
    TimeManager();
    ~TimeManager();
    bool isUpdating;
    static TimeManager* instance;
    cocos2d::Sprite* weatherIcon;
    WeatherType currentWeather;
    // 核心数据
    GameTime currentTime;
    float accumulatedTime; // 累积时间

    // UI 控件
    cocos2d::Node* uiContainer;   // 整个时钟的容器
    cocos2d::Sprite* clockBg;       // 背景图
    cocos2d::Node* clockHand;     // 指针
    cocos2d::Label* timeLabel;     // 时间文字
    cocos2d::Label* dateLabel;     // 日期文字
    cocos2d::Label* moneyLabel;    // 金币文字

    // 内部方法
    void createUI();
    void refreshUI(); // 刷新显示
};

#endif // __TIME_MANAGER_H__
