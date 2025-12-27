/****************************************************************
 * Project Name:  StardewValley
 * File Name:     TimeManager.h
 * File Function: TimeManager类的声明
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "cocos2d.h"
#include "Constant.h" // 引用常量
#include "GameTime.h"
#include <functional> 

 // 保持您要求的原始引用路径
#include "../Weather/WeatherManager.h"
#include "../Money/Money.h"

class TimeManager : public cocos2d::Node {
public:
    // 单例获取
    static TimeManager* getInstance();
    // 单例销毁
    static void destroyInstance();

    // 初始化
    virtual bool init();

    // 时间更新逻辑
    void update(float dt) override;

    // 获取当前时间数据
    GameTime getCurrentTime() const { return _currentTime; }

    // 切换更新状态 (暂停/恢复时间)
    void changeUpdateStatus();

    // 开启新的一天
    void startNextDay();

    // 触发强制昏迷
    void triggerPassOut();

    // 开启睡觉流程动画
    void startSleepSequence();

    // 天数开始回调
    std::function<void()> onDayStartCallback; 

    // 设定时间
    void setTime(const GameTime& time)；
private:
    TimeManager();
    ~TimeManager();

    // 内部UI方法
    void createUI();
    void refreshUI();

private:
    static TimeManager* _instance;

    // 状态标志
    bool _isTransitioning; // 是否正在转场
    bool _isInputAllowed;  // 是否允许输入
    bool _isUpdating;      // 是否正在更新时间

    // 核心数据
    GameTime _currentTime;
    float _accumulatedTime; // 累积时间

    // UI 控件
    cocos2d::Node* _uiContainer;    // 容器
    cocos2d::Sprite* _clockBg;      // 背景
    cocos2d::Sprite* _clockHand;    // 指针
    cocos2d::Label* _timeLabel;     // 时间文本
    cocos2d::Label* _dateLabel;     // 日期文本
    cocos2d::Label* _moneyLabel;    // 金币文本
    cocos2d::Sprite* _weatherIcon;  // 天气图标
};


#endif // __TIME_MANAGER_H__
