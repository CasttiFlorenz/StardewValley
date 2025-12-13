#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "cocos2d.h"
#include "GameTime.h"
#include "WeatherManager.h"

class TimeManager : public cocos2d::Node {
public:
    static TimeManager* getInstance();
    virtual bool init(); // 简化初始化，参数写死在内部
    void update(float dt) override; // 必须有update才能走时间

    // 获取当前时间数据
    GameTime getCurrentTime() const { return currentTime; }

private:
    TimeManager();
    ~TimeManager();
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